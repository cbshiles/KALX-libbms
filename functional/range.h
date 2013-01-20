// range.h - multi-dimensional range functions
// Copyright (c) 2013 KALX, LLC. All rights reserved.
#pragma once
#include <vector>

namespace range {

	typedef ptrdiff_t I;

	// wrap pre-allocated memory
	template<class T>
	class wrapper {
		size_t n_;
		T* t_;
	public:
		wrapper(size_t n, T* t)
			: n_(n), t_(t)
		{ }
		wrapper(const wrapper& a)
			: n_(a.n_), t_(a.t_)
		{ }
		wrapper& operator=(const wrapper& a)
		{
			if (this != &a) {
				n_ = a.n_;
				t_ = a.t_;
			}

			return *this;
		}
	virtual ~wrapper()
		{ }

		size_t size(void) const
		{
			return n_;
		}
		operator T*()
		{
			return t_;
		}
		T* begin(void)
		{
			return t_;
		}
		const T* begin(void) const
		{
			return t_;
		}
		T* end(void)
		{
			return t_ + n_;
		}
		const T* end(void) const
		{
			return t_ + n_;
		}
		bool operator==(const wrapper<T>& u) const
		{
			return (n_ == u.size() && begin() == u.begin()) || std::equal(begin(), end(), u.begin());
		}
		bool operator<(const wrapper<T>& u) const
		{
			return std::lexicographical_compare(begin(), end(), u.begin(), u.end());
		}
		operator const T*() const
		{
			return t_;
		}
		// cyclic 0-based index
		T operator[](I i) const
		{
			return t_[i >= 0 ? i%n_ : n_ - 1 - (-i - 1)%n_];
		}
		T& operator[](I i)
		{
			return t_[i >= 0 ? i%n_ : n_ - 1 - (-i - 1)%n_];
		}
	};

	// store memory and hook up wrapper
	template<class T>
	struct holder : private std::vector<T>, public wrapper<T> {
		holder(size_t n)
			: std::vector<T>(n), wrapper<T>(n, &std::vector<T>::operator[](0))
		{ }
		holder(size_t n, const T* t)
			: std::vector<T>(t, t + n), wrapper<T>(n, &std::vector<T>::operator[](0))
		{ }
		holder(const holder& a)
			: std::vector<T>(a.begin(), a.end()), wrapper<T>(a.size(), &std::vector<T>::operator[](0))
		{ }
		holder& operator=(const holder& a)
		{
			if (this != &a) {
				std::vector<T>::resize(a.size())
				n_ = a.size();
				t_ = &std::vector<T>::operator[](0)l
			}

			return *this;
		}
		~holder()
		{ }

		// override ambiguous members
		size_t size(void) const
		{
			return wrapper<T>::size();
		}
		T* begin(void)
		{
			return wrapper<T>::begin();
		}
		const T* begin(void) const
		{
			return wrapper<T>::begin();
		}
		T* end(void)
		{
			return wrapper<T>::end();
		}
		const T* end(void) const
		{
			return wrapper<T>::end();
		}
		T operator[](I i) const
		{
			return wrapper<T>::operator[](i);
		}
		T& operator[](I i)
		{
			return wrapper<T>::operator[](i);
		}
	};

	template<class T>
	class sequence : public holder<T> {
	public:
		// start, start + step, ..., stop inclusive
		sequence(I start, I stop, I step = 1)
			: holder<T>(1 + (stop - start)/step)
		{
			for (size_t i = 0; i < size(); ++i)
				operator[](i) = start + i*step;
		}
	};

	template<class T>
	inline wrapper<T> take(I n, wrapper<T>& a)
	{
		return n >= 0 ? wrapper<T>(n, a) : wrapper<T>(-n, a + a.size() + n);
	}

	template<class T>
	inline wrapper<T> drop(I n, wrapper<T>& a)
	{
		return n >= 0 ? wrapper<T>(a.size() - n, a + n) : wrapper<T>(a.size() + n, a);
	}

	inline std::function<I(I)> stride(I s, I o = 0)
	{
		return [s,o](I i) { return i*s + o; };
	}

	template<class T>
	inline std::function<size_t(size_t i)> mask(const wrapper<T>& m)
	{
		return [m](size_t i) -> size_t {
			for (size_t j = 0; j < m.size(); ++j) {
				if (m[j] && !i--)
					return j;
			}

			return static_cast<size_t>(-1);
		};
	}

	template<class T>
	inline holder<size_t>&& grade(const wrapper<T>& a, I n = 0)
	{
		sequence<size_t> b(0, a.size() - 1);

		std::function<bool(size_t, size_t)> cmp;
		if (n >= 0)
			cmp = [a](size_t i, size_t j) { return a[i] < a[j];};
		else
			cmp = [a](size_t i, size_t j) { return a[i] > a[j];};

		if (n == 0 || n == -1)
			std::sort(b.begin(), b.end(), cmp);
		else
			std::partial_sort(b.begin(), b.begin() + (n > 0 ? n : -n), b.end(), cmp);

		return std::move(b);
	}

#ifdef _DEBUG

#include "../include/ensure.h"

template<class T>
inline void test_range_index(void)
{
	T _t[] = {1,2,3};
	wrapper<T> t(3, _t);

	sequence<T> s(1, 3);
	ensure (t == s);

	ensure (t[0] == _t[0]);
	ensure (t[-1] == _t[2]);
	ensure (t[3] == _t[0]);
	ensure (t[-3] == _t[0]);
	ensure (t[100] == _t[1]);
}

template<class T>
inline void test_range_take(void)
{
	T _t[] = {1,2,3};
	wrapper<T> t(3, _t);

	ensure (take(2,t)[0] == _t[0]);
	ensure (take(2,t)[1] == _t[1]);
	ensure (take(2,t)[2] == _t[0]);
	ensure (take(2,t)[-1] == _t[1]);

	ensure (take(-2,t)[0] == _t[1]);
	ensure (take(-2,t)[1] == _t[2]);
	ensure (take(-2,t)[2] == _t[1]);
	ensure (take(-2,t)[-1] == _t[2]);
}

template<class T>
inline void test_range_drop(void)
{
	T _t[] = {1,2,3};
	wrapper<T> t(3, _t);

	ensure (drop(1,t)[0] == _t[1]);
	ensure (drop(1,t)[1] == _t[2]);
	ensure (drop(1,t)[2] == _t[1]);
	ensure (drop(1,t)[-1] == _t[2]);

	ensure (drop(-1,t)[0] == _t[0]);
	ensure (drop(-1,t)[1] == _t[1]);
	ensure (drop(-1,t)[2] == _t[0]);
	ensure (drop(-1,t)[-1] == _t[1]);
}

template<class T>
inline void test_range_stride(void)
{
	T _t[] = {1,2,3};
	wrapper<T> t(3, _t);

	ensure (t[stride(2)(0)] == _t[0]);
	ensure (t[stride(2)(1)] == _t[2]);
}

template<class T>
inline void test_range_mask(void)
{
	T _t[] = {1,2,3};
	wrapper<T> t(3, _t);

	bool m[] = {true, false, true};
	ensure (t[mask(wrapper<bool>(3, m))(0)] == _t[0]);
	ensure (t[mask(wrapper<bool>(3, m))(1)] == _t[2]);
}

template<class T>
inline void test_range_grade(void)
{
	T _t[] = {3,1,4,2};
	wrapper<T> t(4, _t);

	holder<size_t> i = grade<T>(t);
	for (T j = 0; j < 4; ++j)
		ensure (t[i[j]] == j);
}

} // namespace range

inline void test_range(void)
{
	range::test_range_index<int>();
	range::test_range_take<double>();
	range::test_range_drop<float>();
	range::test_range_stride<size_t>();
	range::test_range_mask<long>();
	range::test_range_grade<int>();
}

#endif