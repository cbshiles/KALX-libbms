// array_iterator.h - iterator with function applied to each item
// Copyright (c) 2013 KALX, LLC. All rights reserved.
#pragma once
#include <cstddef>
#include <iterator>
#include <utility>

using namespace std::rel_ops;

namespace functional {

	// array of value type T for pre-allocated memory
	template<class T>
	class array_iterator : public std::iterator<std::random_access_iterator_tag, T> {
	protected:
		T *b_, *e_, *i_;
	public:
		array_iterator(T* b, T* e) 
			: b_(b), e_(e), i_(b)
		{ }
		array_iterator(size_t n, T* a)
			: b_(a), e_(a + n), i_(a)
		{ }
		array_iterator(const array_iterator& i)
			: b_(i.b_), e_(i.e_), i_(i.i_)
		{
		}
		array_iterator& operator=(const array_iterator& i)
		{
			if (this != &i) {
				b_ = i.b_;
				e_ = i.e_;
				i_ = i.i_;
			}

			return *this;
		}
		~array_iterator()
		{ }

		bool operator==(const array_iterator& i) const
		{
			return b_ == i.b_ && e_ == i.e_ && i_ == i.i_;
		}
		bool operator<(const array_iterator& i) const
		{
			return b_ == i.b_ && e_ == i.e_ && i_ < i.i_; 
		}

		T* begin(void)
		{
			return b_;
		}
		const T* begin(void) const
		{
			return b_;
		}
		T* end(void)
		{
			return e_;
		}
		const T* end(void) const
		{
			return e_;
		}

		operator T*(void)
		{
			return i_;
		}
		operator const T*(void) const
		{
			return i_;
		}
		T operator*() const
		{
			return *i_;
		}
		T& operator*()
		{
			return *i_;
		}
		array_iterator& operator++(void)
		{
			++i_;

			return *this;
		}
		array_iterator operator++(int)
		{
			array_iterator<T> i(*this);

			++i_;

			return i;
		}
		array_iterator& operator+=(ptrdiff_t n)
		{
			i_ += n;

			return *this;
		}
		array_iterator& operator-=(ptrdiff_t n)
		{
			i_ -= n;

			return *this;
		}
		array_iterator operator+(ptrdiff_t n) const
		{
			array_iterator<T> i(*this);

			i += n;

			return i;
		}
		array_iterator operator-(ptrdiff_t n) const
		{
			array_iterator<T> i(*this);

			i -= n;

			return i;
		}
	};
	template<class T>
	inline array_iterator<T> array(T* b, T* e)
	{
		return array_iterator<T>(b, e);
	}

} // namespace functional
