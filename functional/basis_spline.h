// basis_spline.h - basis splines
// Copyright (c) 2013 KALX, LLC. All rights reserved.
#pragma once
#include <functional>

namespace functional {

//                       x - t(i)              t(i+k+1) - x
//     b(i,k+1)(x)  =  -----------b(i,k)(x) + ---------------b(i+1,k)(x)
//                     t(i+k)-t(i)            t(i+k+1)-t(i+1)
	
	// right continuous i-th basis spline of order k having n knot points t
	template<class T, class U>
	inline std::function<T(T)> basis_spline(size_t i, size_t k, size_t n, const U* t)
	{
		ensure (i + 1 < n);

		if (k == 0) { // order 0
			return [i,t](T x) { return 1*(t[i] <= x && x < t[i + 1]); };
		}

		return [i,k,n,t](T x) -> T {
			T b  = basis_spline<T,U>(i, k - 1, n, t)(x);
			T b_ = basis_spline<T,U>(i + 1, k - 1, n, t)(x);
			T dt  = static_cast<T>(t[i + k] - t[i]);
			T dt_ = static_cast<T>(t[i + k + 1] - t[i + 1];

			return (x - t[i])*b/dt + (t[i + k + 1] - x)*b_/dt_);
		};
	}

	template<size_t K>
	struct basis_spline {
		template<class T, class U>
		T value(T x, size_t i, size_t n, const U* t)
		{
			T b  = basis_spline<T,U>::value<K-1>(i, n, t)(x);
			T b_ = basis_spline<T,U>::value<K-1>(i + 1, n, t)(x);
			T dt  = static_cast<T>(t[i + k] - t[i]);
			T dt_ = static_cast<T>(t[i + k + 1] - t[i + 1];

			return (x - t[i])*b/dt + (t[i + k + 1] - x)*b_/dt_);
		}
	};
	template<>
	struct basis_spline<0> {
		template<class T, class U>
		T value(T x, size_t i, size_t n, const U* t)
		{
			return 1*(t[i] <= x && x < t[i + 1]); };
		}
	};

	template<class T, class U>
	inline std::function<T(T)> bspline(const U* a, size_t k, size_t n, const U* t)
	{
		return [a,k,n,t](T x) {
			T b(0);

			//!! localize based on x first!!!
			for (size_t i = 0; i < n - k; ++i)
				b += a[i]*basis_spline(i, k, n, t)(x);

			return b;
		};
	}

} // namespace functional
