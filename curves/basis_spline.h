// basis_spline.h - basis splines
// Copyright (c) 2013 KALX, LLC. All rights reserved.
#pragma once
#include "../include/ensure.h"
#include <algorithm>
#include <functional>

namespace curves {
namespace basis_spline {

//                       x - t(i)               t(i+k+1) - x
//     B(i,k+1)(x)  =  ----------- B(i,k)(x) + --------------- B(i+1,k)(x)
//                     t(i+k)-t(i)             t(i+k+1)-t(i+1)
	
	// right continuous i-th basis spline of order k having n knot points t
	template<class T, class U>
	inline std::function<T(size_t,T)> value(size_t k, size_t n, const U* t)
	{
		if (k == 0) { // order 0 - piecewise constant
			return [n,t](size_t i, T x) -> T { 
				ensure (i + 1 < n);
			
				return 1*(t[i] <= x && x < t[i + 1]); 
			};
		}

		return [k,n,t](size_t i, T x) -> T 
		{
			ensure (i + k + 1 < n);

			if (x < t[i] || x >= t[i + k + 1])
				return 0;

			auto B = value<T,U>(k - 1, n, t);
			T b  = B(i, x);
			T b_ = B(i + 1, x);
			T dt  = static_cast<T>(t[i + k] - t[i]);
			T dt_ = static_cast<T>(t[i + k + 1] - t[i + 1]);
			T b_dt = dt ? b/dt : 0;
			T b_dt_ = dt_ ? b_/dt_ : 0;

			return (x - t[i])*b_dt + (t[i + k + 1] - x)*b_dt_;
		};
	}

	template<class T, class U>
	inline std::function<T(size_t,T)> derivative(size_t k, size_t n, const U* t)
	{
		return [k,n,t](size_t i, T x) -> T
		{
			auto B =value<T,U>(k - 1, n, t);

			return k*B(i, x)/(t[i + k] - t[i]) - k*B(i + 1, x)/(t[i + k + 1] - t[i + 1]);
		};
	}

	template<class T, class U>
	inline std::function<T(size_t,T)> integral(size_t k, size_t n, const U* t)
	{
		return [k,n,t](size_t i, T x) -> T 
		{
			T I(0);

			auto B = value<T,U>(k + 1, n, t);

			for (size_t j = i; j + k + 2 < n; ++j)
				I += B(j, x);

			return I*(t[i + k + 1] - t[i])/(k + 1);
		};
	}

} // namespace basis_spline
} // namespace curve
