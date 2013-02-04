// apply_iterator.h - iterator with function applied to each item
// Copyright (c) 2013 KALX, LLC. All rights reserved.
#pragma once
#include <iterator>

namespace functional {

	template<class F, class I>
	class apply_iterator : public std::iterator<std::input_iterator_tag, typename std::iterator_traits<I>::value_type> {
		apply_iterator& operator=(const apply_iterator& i);
	protected:
		~apply_iterator()
		{ }
		operator I(void)
		{
			return i_;
		}
		operator I(void) const
		{
			return i_;
		}
		value_type operator*() const
		{
			return f_(*i_);
		}
		apply_iterator& operator++(void)
		{
			++i_;

			return *this;
		}
		apply_iterator operator++(int)
		{
			apply_iterator i(f_, i_);

			++i_;

			return i;
		}
	};
	template<class F, class I>
	inline apply_iterator<F,I> apply(const F& f, I i)
	{
		return apply_iterator<F,I>(f, i);
	}

} // namespace functional