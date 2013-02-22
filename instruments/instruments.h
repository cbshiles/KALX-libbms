// instruments.h - fixed income instruments
// Copyright (c) 2013 KALX, LLC. All rights reserved.
#pragma once

namespace instruments {

	// time, cash flow, and date type
	template<class T, class C, class D = void*>
	class fixed {
	protected:
		size_t n_;
		const T* t_;
		const C* c_;
	public:
		fixed(size_t n, const T* t, const C* c)
			: n_(n), t_(t), c_(c)
		{ }
		virtual ~fixed()
		{ }
		// fix cash flows given date and coupon
		virtual fixed& fix(D, C)
		{
			return *this;
		}
		size_t size(void) const
		{
			return n_;
		}
		const T* time(void) const
		{
			return t_;
		}
		const C* flow(void) const
		{
			return c_;
		}
	};

} // namespace instruments