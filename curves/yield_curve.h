// yield_curve.h - piecewise flat bootstrapped yield curve
// Copyright (c) 2011-2012 KALX, LLC. All rights reserved. No warranty made.
#pragma once
#include <vector>
#include "bootstrap.h"

namespace curves {
namespace pwflat {

	template<class T = double, class F = double>
	class yield_curve {
		std::vector<T> t_;
		std::vector<F> f_;
	public:
		yield_curve(size_t n = 0, const T* t = 0, const F* f = 0)
			: t_(t, t + n), f_(f, f + n)
		{ }
		// use this to call value, spot, discount, etc.
		curves::pwflat::forward<T,F> forward(void)
		{
			return t_.size() ? curves::pwflat::forward<T,F>(t_.size(), &t_[0], &f_[0]) : curves::pwflat::forward<T,F>();
		}
		const curves::pwflat::forward<T,F>& forward(void) const
		{
			return t_.size() ? curves::pwflat::forward<T,F>(t_.size(), &t_[0], &f_[0]) : curves::pwflat::forward<T,F>();
		}
		// cash deposit
		yield_curve& add(T t, F c)
		{
			f_.push_back(forward().bootstrap1(t, c));
			t_.push_back(t);

			return *this;
		}
		// forward rate agreement
		yield_curve& add(T t0, F c0, T t1, F c1)
		{
			f_.push_back(forward().bootstrap2(t0, c0, t1, c1));
			t_.push_back(t1);

			return *this;
		}
		// general cash flows
		yield_curve& add(size_t m, const T* u, const F* c, F p = 0)
		{
			f_.push_back(forward().bootstrap(m, u, c, p));
			t_.push_back(u[m-1]);

			return *this;
		}
	};

} // namespace pwflat
} // namespace curves