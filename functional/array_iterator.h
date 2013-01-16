// array_iterator.h - iterator with function applied to each item
// Copyright (c) 2013 KALX, LLC. All rights reserved.
#pragma once
#include <cstddef>
#include <iterator>
#include <utility>

using namespace std::rel_ops;

namespace functional {

	// Why doesn't this already exist?
	template<class I>
	inline I iterator_advance(I i, typename std::iterator_traits<I>::difference_type n)
	{
		I i_(i);

		std::advance(i_, n);

		return i_;
	}

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
	inline array_iterator<T> make_array_iterator(T* b, T* e)
	{
		return array_iterator<T>(b, e);
	}

	// general iterator adaptor
	template<class I>
	class iterator_adaptor : public std::iterator<
		typename std::iterator_traits<I>::iterator_category,
		typename std::iterator_traits<I>::value_type,
		typename std::iterator_traits<I>::difference_type,
		typename std::iterator_traits<I>::pointer,
		typename std::iterator_traits<I>::reference> {
	protected:
		I b_, e_, i_;
	public:
		iterator_adaptor(I b, I e) 
			: b_(b), e_(e), i_(b)
		{ }
		iterator_adaptor(size_t n, I a)
			: b_(a), e_(iterator_advance(a, n)), i_(a)
		{ }
		iterator_adaptor(const iterator_adaptor& i)
			: b_(i.b_), e_(i.e_), i_(i.i_)
		{
		}
		iterator_adaptor& operator=(const iterator_adaptor& i)
		{
			if (this != &i) {
				b_ = i.b_;
				e_ = i.e_;
				i_ = i.i_;
			}

			return *this;
		}
		~iterator_adaptor()
		{ }

		bool operator==(const iterator_adaptor& i) const
		{
			return b_ == i.b_ && e_ == i.e_ && i_ == i.i_;
		}
		bool operator<(const iterator_adaptor& i) const
		{
			return b_ == i.b_ && e_ == i.e_ && i_ < i.i_; 
		}

		iterator_adaptor begin(void)
		{
			return b_;
		}
		const iterator_adaptor begin(void) const
		{
			return b_;
		}
		iterator_adaptor end(void)
		{
			return e_;
		}
		const iterator_adaptor end(void) const
		{
			return e_;
		}
/*
		operator T*(void)
		{
			return i_;
		}
		operator const T*(void) const
		{
			return i_;
		}
*/		value_type operator*() const
		{
			return *i_;
		}
		reference operator*()
		{
			return *i_;
		}
		const value_type operator[](difference_type n) const
		{
			I i(i_);

			std::advance(i, n);

			return *i;
		}
		reference operator[](difference_type n)
		{
			I i(i_);

			std::advance(i, n);

			return *i;
		}

		iterator_adaptor& operator++(void)
		{
			++i_;

			return *this;
		}
		iterator_adaptor operator++(int)
		{
			iterator_adaptor<I> i(*this);

			++i_;

			return i;
		}
		iterator_adaptor& operator+=(difference_type n)
		{
			std::advance(i_, n);

			return *this;
		}
		iterator_adaptor& operator-=(difference_type n)
		{
			std::advance(i_, -n);

			return *this;
		}
		iterator_adaptor operator+(difference_type n) const
		{
			iterator_adaptor<I> i(*this);

			i += n;

			return i;
		}
		iterator_adaptor operator-(difference_type n) const
		{
			iterator_adaptor<I> i(*this);

			i -= n;

			return i;
		}
	};
	template<class I>
	inline iterator_adaptor<I> make_iterator_adaptor(I b, I e)
	{
		return iterator_adaptor<I>(b, e);
	}

	template<class I>
	class stride_iterator : public std::iterator<
		typename std::iterator_traits<I>::iterator_category,
		typename std::iterator_traits<I>::value_type,
		typename std::iterator_traits<I>::difference_type,
		typename std::iterator_traits<I>::pointer,
		typename std::iterator_traits<I>::reference> {
	protected:
		I i_;
		difference_type stride_;
	public:
		stride_iterator(I i, difference_type stride = 1, difference_type offset = 0)
			: i_(iterator_advance(i, offset)), stride_(stride)
		{ }
		value_type operator*() const
		{
			return *i_;
		}
		reference operator*()
		{
			return *i_;
		}
		value_type operator[](difference_type n) const
		{
			I i(i_);

			std::advance(i, n*stride_)

			return *i_;
		}
		reference operator[](difference_type n)
		{
			I i(i_);

			std::advance(i, n*stride_)

			return *i_;
		}

		stride_iterator& operator++()
		{
			std::advance(i_, stride_);

			return *this;
		}
		stride_iterator operator++(int) const
		{
			I i(i_);

			std::advance(i, stride_);

			return stride_iterator(i, stride_);
		}
		stride_iterator& operator+=(difference_type n)
		{
			std::advance(i_, n*stride_);

			return *this;
		}
	};

} // namespace functional
