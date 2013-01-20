// polynomial.h - polynomials
// Copyright (c) 2013 KALX, LLC. All rights reserved.
// Parameterize by argument T - float, double, dual number, matrix; and coefficients U - typically a scalar
#pragma once
#include <functional>

namespace functional {
namespace polynomial {

	// p[0] + p[1]*x + ...  = p[0] + x*(p[1] + ...)
	template<class T, class U>
	inline std::function<T(T)> horner(size_t n, const U* p)
	{
		return [n,p](const T& x) -> T {
			return static_cast<T>(n == 0 ? 0 : (n == 1 || x == 0) ? p[0] : p[0] + x*horner<T,U>(n - 1, p + 1)(x));
		};
	}
		
	// p[0] + p[1] x + p[2]/2! x + ...  + p[n]/n! x^n = p[0] + x (p[1] + x/2 (p[2] + x/3 (p[3] + ... + x/n p[n])))
	template<class T, class U>
	inline std::function<T(T)> taylor(size_t n, const U* p, T f = 1)
	{
		return [n,p,f](const T& x) -> T {
			return static_cast<T>(n == 0 ? 0 : n == 1 ? p[0] : p[0] + (x/f)*taylor<T,U>(n - 1, p + 1, f + 1)(x));
		};
	}


#ifdef _DEBUG

template<class T, class U>
inline void test_functional_polynomial_(void)
{
	U p[3] = {1, 2, 3};
	auto F = polynomial::horner<T,U>(3, p);
	ensure (F(0) == p[0]);
	ensure (F(1) == p[0] + 1*p[1] + 1*p[2]); 
	ensure (F(2) == p[0] + 2*p[1] + 2*2*p[2]); 

	U q[4] = {1, 2, 3, 4};
	U one(1), two(2), three(3);
	auto G = polynomial::taylor<T,U>(4, q);
	ensure (G(0) == q[0]);
	ensure (G(1) == q[0] + one*(q[1] + (one/two)*(q[2] + (one/three)*q[3])));
	ensure (G(2) == q[0] + two*(q[1] + (two/two)*(q[2] + (two/three)*q[3])));
}

inline void test_functional_polynomial(void)
{
	test_functional_polynomial_<double,double>();
	test_functional_polynomial_<double,float>();
}
#endif // _DEBUG


} // namespace polynomial
} // namespace functional

