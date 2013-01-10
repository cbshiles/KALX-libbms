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
	inline std::function<T(size_t,T)> basis_spline(size_t k, size_t n, const U* t)
	{

		if (k == 0) { // order 0
			
			return [n,t](size_t i, T x) -> T { 
				ensure (i + 1 < n);
			
				return 1*(t[i] <= x && x < t[i + 1]); 
			};
		}

		return [k,n,t](size_t i, T x) -> T {
			ensure (i + k + 1 < n);

			auto B = basis_spline<T,U>(k - 1, n, t);
			T b  = B(i, x);
			T b_ = B(i + 1, x);
			T dt  = static_cast<T>(t[i + k] - t[i]);
			T dt_ = static_cast<T>(t[i + k + 1] - t[i + 1]);

			return (x - t[i])*b/dt + (t[i + k + 1] - x)*b_/dt_;
		};
	}

	template<size_t K>
	struct basis_spline_struct {
		template<class T, class U>
		T value(T x, size_t i, size_t n, const U* t)
		{
			T b  = basis_spline_struct<K-1>::value(x, i, n, t);
			T b_ = basis_spline_struct<K-1>::value(x, i + 1, n, t);
			T dt  = static_cast<T>(t[i + K] - t[i]);
			T dt_ = static_cast<T>(t[i + K + 1] - t[i + 1]);

			return (x - t[i])*b/dt + (t[i + K + 1] - x)*b_/dt_;
		}
	};
	template<>
	struct basis_spline_struct<0> {
		template<class T, class U>
		T value(T x, size_t i, size_t n, const U* t)
		{
			return 1*(t[i] <= x && x < t[i + 1]);
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
