// range.h - range functions
// Copyright (c) 2013 KALX, LLC. All rights reserved.
/*
Wrap range of pointers. Commands change wrappers, functions don't.
*/
#pragma once
#include <algorithm>
#include <functional>
#include <iterator>
#include <vector>
#include "../include/ensure.h"

#define nullptr 0

namespace range {

	typedef ptrdiff_t I;

	// cyclic 0-based index in the range [0, n)
	inline I index(I i, I n)
	{
		return (i >= 0) ? (i < n ? i : i%n) : n - 1 - (-i - 1)%n;
	}

	template<class T> class row;
	// wrap pre-allocated memory
	template<class T>
	class wrap {
		friend class row<T>;
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
			return (n_ == u.size() && begin() == u.begin()) || std::equal(begin(), end(), u.begin());
		}
		bool operator<(const wrap<T>& u) const
		{
			return std::lexicographical_compare(begin(), end(), u.begin(), u.end());
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
	};

	// allocate vector to wrap
	template<class T>
	class hold : private std::vector<T>, public wrap<T> {
	public:
		using wrap<T>::size;
		using wrap<T>::begin;
		using wrap<T>::end;
		using wrap<T>::operator[];

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
			}

			return *this;
		}
		hold& operator=(hold&& r)
		{
			if (this != &r) {
				std::vector<T>::operator=(r);
				wrap<T>::operator=(r);
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
	};

	template<class T>
	class row_iterator : public std::iterator<std::random_access_iterator_tag, typename row<T>> {
		row<T> r_;
	public:
		row_iterator(row<T>* pr = nullptr)
			: pr_(pr)
		{ }
		row_iterator(const row_iterator& ir)
			: pr_(ir.pr_)
		{ }
		row_iterator(row_iterator&& ir)
			: pr_(ir.pr_)
		{ }
		row_iterator& operator=(const row_iterator& ir)
		{
			if (this != &ir)
				pr_ = ir.pr_;

			return *this;
		}

		// trivial iterator
		row<T>& operator*(void)
		{
			return *pr_;
		}
		const row<T>& operator*(void) const
		{
			return *pr_;
		}

		// forward iterator
		row_iterator& operator++(void)
		{
			++*pr_;

			return *this;
		}
		row_iterator operator++(int)
		{
			return row_iterator(&*pr_++);
		}

		// bidirectional iterator
		row_iterator& operator--(void)
		{
			--*pr_;

			return *this;
		}
		row_iterator operator--(int)
		{
			return row_iterator(&*pr_--);
		}
		ptrdiff_t operator-(const row_iterator& r) const
		{
			return *pr_ - *r.pr_;
		}

		// random access
		row_iterator& operator+=(size_t n)
		{
			*pr += n;

			return *this;
		}
		row_iterator& operator-=(size_t n)
		{
			*pr -= n;

			return *this;
		}
	};

	// one row of a 2-dim array
	template<class T>
	class row {
		size_t c_; // columns
		wrap<T> r_;
	public:
		row()
			: c_(0)
		{ }
		row(wrap<T> r, size_t c)
			: r_(r), c_(c)
		{ }
		row(const row& r)
			: r_(r.r_), c_(r.c_)
		{ }
		row& operator=(const row& r)
		{
			if (this != &r) {
				r_ = r.r_;
				c_ = r.c_;
			}

			return *this;
		}
		~row()
		{ }
		
		size_t size(void) const
		{
			return c_;
		}
		
		bool operator==(const row& r)
		{
			return c_ == r.c_ && r_ == r.r_;
		}
		bool operator<(const row& r)
		{
			return c_ < r.c_ || c_ = r.c_ && r_ < r.r_;
		}

		T operator[](size_t i) const
		{
			return r_[i];
		}
		T& operator[](size_t i)
		{
			return r_[i];
		}

		row& operator++(void)
		{
			r_.t_ += c_;

			return *this;
		}
		row operator++(int)
		{
			row r(*this);

			operator++();

			return r;
		}
		row& operator--(void)
		{
			r_.t_ -= c_;
		}
		row operator--(int)
		{
			row r(*this);

			operator--;

			return r;
		}
		row& operator+=(size_t n)
		{
			r_.t_ += n*c_;

			return *this;
		}
		row operator+(size_t n)
		{
			row r(*this);

			return r += n;
		}
		row& operator-=(size_t n)
		{
			r_.t_ -= n*c_;

			return *this;
		}
		row operator-(size_t n)
		{
			row r(*this);

			return r -= n;
		}
		ptrdiff_t operator-(const row& r) const
		{
			ensure (c_ == r.c_);

			return (r_ - r.r_)/c_;
		}
		row_iterator<T> begin(void)
		{
			return row_iterator<T>(this);
		}
		row_iterator<T> end(void)
		{
			row<t> r(r_);

			r.r_ += r_.size();

			return row_iterator<T>(r);
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

 
/*	class sequence : public hold<T> {
	public:
		// start, start + step, ..., stop inclusive
		sequence(I start, I stop, I step = 1)
			: hold<T>((stop - start + 1)/step)
		{
			for (size_t i = 0; i < n_; ++i)
				operator[](i) = start + i*step;
		}
		sequence(const sequence& s)
			: hold<T>(s)
		{
		}
		sequence(sequence&& s)
			: hold<T>(std::move(s))
		{ }
		sequence& operator=(const sequence& s)
		{
			if (this != &r) {
				std::hold<T>::operator=(s);
			}

			return *this;
		}
		sequence& operator=(sequence&& s)
		{
			if (this != &s) {
				std::hold<T>::operator=(s);
				wrap<T>::operator=(s);
			}

			return *this;
		}
		~sequence()
		{ }
	};
*/
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

	template<class I>
	inline std::function<I(I)> stride(I s, I o = 0)
	{
		return [s,o](I i) { return i*s + o; };
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

		return b;
	}

} // namespace range