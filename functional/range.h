// range.h - range functions
// Copyright (c) 2013 KALX, LLC. All rights reserved.
/*
Wrap range of pointers. Commands change wrappers, functions don't.
*/
#pragma once
#include <algorithm>
#include <functional>
#include <iterator>
#include <utility>
#include <vector>
#include "../include/ensure.h"

#define nullptr 0

using namespace std::rel_ops;

namespace range {

	typedef ptrdiff_t I;

	// cyclic 0-based index in the range [0, n)
	inline I index(I i, I n)
	{
		return (i >= 0) ? (i < n ? i : i%n) : n - 1 - (-i - 1)%n;
	}

	template<class T> class row;
	template<class T> class row_iterator;
	// wrap pre-allocated memory
	template<class T>
	class wrap {
		friend class row<T>;
		friend class row_iterator<T>;
	protected:
		size_t n_;
		T* t_;
	public:
		wrap(size_t n = 0, T* t = nullptr)
			: n_(n), t_(t)
		{ }
		wrap(const wrap& a)
			: n_(a.n_), t_(a.t_)
		{ }
		wrap(wrap&& a)
			: n_(a.n_), t_(a.t_)
		{
			a.n_ = 0;
			a.t_ = 0;
		}
		wrap& operator=(const wrap& a)
		{
			if (this != &a) {
				n_ = a.n_;
				t_ = a.t_;
			}

			return *this;
		}
		wrap& operator=(wrap&& a)
		{
			if (this != &a) {
				n_ = a.n_;
				t_ = a.t_;
				a.n_ = 0;
				a.t_ = 0;
			}

			return *this;
		}
		virtual ~wrap()
		{ }

		size_t size(void) const
		{
			return n_;
		}
		operator T*()
		{
			return t_;
		}
		operator const T*() const
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

		bool operator==(const wrap<T>& u) const
		{
			return n_ == u.n_ && (begin() == u.begin()) || std::equal(begin(), end(), u.begin());
		}
		bool operator<(const wrap<T>& u) const
		{
			return n_ < u.n_ || (n_ == u.n_ && std::lexicographical_compare(begin(), end(), u.begin(), u.end()));
		}

		// cyclic 0-based index
		T operator[](I i) const
		{
			return t_[index(i,n_)];
		}
		T& operator[](I i)
		{
			return t_[index(i,n_)];
		}

		wrap operator++(void)
		{
			n_--;
			t_++;

			return *this;
		}
		wrap operator+=(size_t n)
		{
			n_ -= n;
			t_ += n;

			return *this;
		}
	};

	// allocate vector to wrap
	template<class T>
	class hold : private std::vector<T>, public wrap<T> {
	public:
		using wrap<T>::size;
		using wrap<T>::begin;
		using wrap<T>::end;
		using wrap<T>::operator[];
		using wrap<T>::operator==;
		using wrap<T>::operator<;

		hold()
			: std::vector<T>(), wrap<T>()
		{ }
		explicit hold(size_t n)
			: std::vector<T>(n), wrap<T>(n, &std::vector<T>::operator[](0))
		{ }
		hold(size_t n, T* t)
			: std::vector<T>(t, t + n), wrap<T>(n, &std::vector<T>::operator[](0))
		{ }
		hold(const hold& r)
			: std::vector<T>(r), wrap<T>(r.n_, &std::vector<T>::operator[](0))
		{ }
		hold(hold&& r)
			: std::vector<T>(std::move(r)), wrap<T>(r.n_, &std::vector<T>::operator[](0))
		{ }
		hold& operator=(const hold& r)
		{
			if (this != &r) {
				std::vector<T>::operator=(r);
				wrap<T>(*this);
			}

			return *this;
		}
		hold& operator=(hold&& r)
		{
			if (this != &r) {
				std::vector<T>::operator=(r);
				wrap<T>::operator=(*this);
			}

			return *this;
		}
		~hold()
		{ }
		void push_back(T t)
		{
			std::vector<T>::push_back(t);
			++n_;
			t_ = &operator[](0);
		}
		void append(size_t n, const T* t)
		{
			std::vector<T>::insert(std::vector<T>::end(), t, t + n)
			n_ += n;
			t_ = &operator[](0);
		}
	};

	// count by 1
	class counter {
		size_t n_;
	public:
		counter(size_t n = 0)
			: n_(n)
		{ }
		operator size_t()
		{
			return n_++;
		}
		void reset(size_t n = 0)
		{
			n_ = n;
		}
	};

	template<class T>
	inline void sequence(wrap<T>& t, T start, T step = 1)
	{
		counter n(0);

		std::transform(t.begin(), t.end(), t.begin(), [&n,start,step](T t) -> T { return start + n*step; });
	}
	template<class T>
	inline hold<T> sequence(T start, T stop, T step = 1)
	{
		T n = (stop - start + 1)/step;
		ensure (n >= 0);

		hold<T> t(static_cast<size_t>(n));

		sequence(t, start, step);
		
		return t;
	}
	
	template<class T>
	inline wrap<T> take(I n, wrap<T>& a)
	{
		ensure (n <= static_cast<I>(a.size()));
		ensure (-n <= static_cast<I>(a.size()));

		return n >= 0 ? wrap<T>(n, a) : wrap<T>(-n, a + a.size() + n);
	}

	template<class T>
	inline wrap<T> drop(I n, wrap<T>& a)
	{
		return n >= 0 ? wrap<T>(a.size() - n, a + n) : wrap<T>(a.size() + n, a);
	}

	template<class T>
	inline std::function<size_t(size_t i)> mask(const wrap<T>& m)
	{
		return [m](size_t i) -> size_t {
			for (size_t j = 0; j < m.size(); ++j) {
				if (m[j] && !i--)
					return j;
			}

			return static_cast<size_t>(-1);
		};
	}

	// grade n elements of a, decreasing if n < 0
	template<class T>
	inline hold<size_t> grade(const wrap<T>& a, I n = 0)
	{
		hold<size_t> b = sequence<size_t>(0, a.size() - 1);

		std::function<bool(size_t, size_t)> cmp;
		if (n >= 0)
			cmp = [a](size_t i, size_t j) { return a[i] < a[j];};
		else
			cmp = [a](size_t i, size_t j) { return a[i] > a[j];};

		if (n == 0 || n == -1)
			std::sort(b.begin(), b.end(), cmp);
		else
			std::partial_sort(b.begin(), b.begin() + (n > 0 ? n : -n), b.end(), cmp);

		return b;
	}

} // namespace range

namespace std{
	template<class T>
	inline void swap(range::row<T>& a, range::row<T>& b)
	{
		a.swap(b);
	}
}