// polynomial.h - polynomials
// Copyright (c) 2013 KALX, LLC. All rights reserved.
// Parameterize by argument T - float, double, dual number, matrix
// and coefficients U are typically scalar
#pragma once
#include <cmath>
#include <functional>
#include "../include/ensure.h"
#include "../numerical/ulp.h"

namespace curves {
namespace polynomial {

	// p[0] + p[1]*x + ...  = p[0] + x*(p[1] + ...)
	template<class T, class U>
	inline std::function<T(T)> horner(size_t n, const U* p)
	{
		return [n,p](const T& x) -> T {
			return static_cast<T>(n == 0 ? 0 : (n == 1 || x == 0)
					? p[0] : p[0] + x*horner<T,U>(n - 1, p + 1)(x));
		};
	}
		
	// p[0] + p[1] x + p[2]/2! x + ...  + p[n-1]/(n-1)! x^(n-1)
	// = p[0] + x (p[1] + x/2 (p[2] + x/3 (p[3] + ... + x/(n-1) p[n-1])))
	template<class T, class U>
	inline std::function<T(T)> taylor(size_t n, const U* p, T f = 1)
	{
		return [n,p,f](const T& x) -> T {
			return static_cast<T>(n == 0 ? 0 : n == 1 ? p[0]
				: p[0] + (x/f)*taylor<T,U>(n - 1, p + 1, f + 1)(x));
		};
	}

} // namespace polynomial
} // namespace curves
