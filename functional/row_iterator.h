// row_iterator.h - iterate over rows of 2-d range
// Copyright (c) 2013 KALX, LLC. All rights reserved.
#pragma once
#include "range.h"

namespace range {

	// one row of a 2-dim array ??? row_view???
	template<class T>
	class row {
		friend class row_iterator<T>;
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
//			ensure (c_ == r.c_);

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
		
		bool operator==(const row& r) const
		{
			return c_ == r.c_ && std::equal(r_.t_, r_.t_ + c_, r.r_.t_);
		}
		bool operator<(const row& r) const
		{
			return c_ < r.c_ || c_ == r.c_ && std::lexicographical_compare(r_.t_, r_.t_ + c_, r.r_.t_, r.r_.t_ + c_);
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

			return *this;
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
			r_.n_ -= n*c_;

			return *this;
		}
		row operator+(size_t n)
		{
			row r(*this);

			r += n;

			return r;
		}
		row& operator-=(size_t n)
		{
			r_.t_ -= n*c_;
			r_.n_ -= n*c_;

			return *this;
		}
		row operator-(size_t n)
		{
			row r(*this);

			r -= n;

			return r;
		}
		ptrdiff_t operator-(const row& r) const
		{
			ensure (c_ == r.c_);

			return (r_ - r.r_)/c_;
		}
		row_iterator<T> begin(void)
		{
			return row_iterator<T>(*this);
		}
		row_iterator<T> end(void)
		{
			row<T> r(*this);

			r += r_.n_/c_;

			return row_iterator<T>(r);
		}

		void swap(row& r)
		{
			ensure (c_ == r.c_);

			if (r_.t_ != r.r_.t_)
				std::swap_ranges(r_.t_, r_.t_ + c_, r.r_.t_);
		}
	};

	template<class T>
	class row_iterator : public std::iterator<std::random_access_iterator_tag, typename row<T>> {
		row<T> r_;
	public:
		row_iterator(row<T> r = row<T>())
			: r_(r)
		{ }
		row_iterator(const row_iterator& ir)
			: r_(ir.r_)
		{ }
		row_iterator(row_iterator&& ir)
			: r_(ir.r_)
		{ }
		row_iterator& operator=(const row_iterator& ir)
		{
			if (this != &ir)
				r_ = ir.r_;

			return *this;
		}

		// pointer comparison
		bool operator==(const row_iterator& ir) const
		{
			return r_.r_.t_ == ir.r_.r_.t_;
		}
		bool operator!=(const row_iterator& ir) const
		{
			return !(r_.r_.t_ == ir.r_.r_.t_);
		}
		bool operator<(const row_iterator& ir) const
		{
			return r_.r_.t_ < ir.r_.r_.t_;
		}

		// trivial iterator
		row<T>& operator*(void)
		{
			return r_;
		}
		const row<T>& operator*(void) const
		{
			return r_;
		}

		// forward iterator
		row_iterator& operator++(void)
		{
			++r_;

			return *this;
		}
		row_iterator operator++(int)
		{
			return row_iterator(r_++);
		}

		// bidirectional iterator
		row_iterator& operator--(void)
		{
			--r_;

			return *this;
		}
		row_iterator operator--(int)
		{
			return row_iterator(r_--);
		}
		ptrdiff_t operator-(const row_iterator& r) const
		{
			return r_ - r.r_;
		}

		// random access
		row_iterator& operator+=(size_t n)
		{
			r_ += n;

			return *this;
		}
		row_iterator& operator-=(size_t n)
		{
			r_ -= n;

			return *this;
		}
		row_iterator operator+(size_t n) const
		{
			row_iterator i(*this);

			i += n;

			return i;
		}
		row_iterator operator-(size_t n) const
		{
			row_iterator i(*this);

			i -= n;

			return i;
		}
	};

} // namespace range