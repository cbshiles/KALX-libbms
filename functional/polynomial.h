// polynomial.h - polynomials
// Copyright (c) 2013 KALX, LLC. All rights reserved.
#pragma once
#include <functional>

namespace functional {
namespace polynomial {

	// p[0] + p[1]*x + ...  = p[0] + x*(p[1] + ...)
	template<class T, class U>
	inline std::function<T(T)> horner(size_t n, const U* p)
	{
		return [n,p](const T& x) -> T {
			return static_cast<T>(n == 0 ? 0 : n == 1 ? p[0] : p[0] + x*horner<T,U>(n - 1, p + 1)(x));
		};
	}
		
	// p[0] + p[1] x + p[2]/2! x + ...  + p[n]/n! x^n = p[0] + x (p[1] + x/2 (p[2] + x/3 (p[3] + ... + x/n p[n])))
	template<class T, class U>
	inline std::function<T(T)> taylor(size_t n, const U* p, double f = 1)
	{
		return [n,p,f](const T& x) -> T {
			return static_cast<T>(n == 0 ? 0 : n == 1 ? p[0] : p[0] + (x/f)*taylor<T,U>(n - 1, p + 1, f + 1)(x));
		};
	}

} // namepsace polynomial
} // namespace functional