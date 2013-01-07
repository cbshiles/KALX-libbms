// tfunctional.cpp - test functional routines
// Copyright (c) 2013 KALX, LLC. All rights reserved. No warranty is made.
#include <cmath>
#include <iostream>
#include "functional.h"

using namespace functional;

template<class T>
void test_apply_iterator(void)
{
	T t[] = {1, 2, 3};

	auto i = apply([](T x) { return x*x; }, t);
	ensure (*i == 1);
	ensure (*++i == 4);
	ensure (*++i == 9);
	ensure (i == t + 2);

	auto i1(i);

	auto j = apply([](T x) { return x*x; }, t);
	ensure (*j++ == 1);
	ensure (*j++ == 4);
	ensure (*j++ == 9);
	ensure (j == t + 3);

	T* pt = t;
	for (auto k = apply([](T x) { return x*x; }, t); k != t + 3; ++k, ++pt)
		ensure (*k == (*pt)*(*pt));

	auto l = apply(static_cast<T(*)(T)>(exp), t);
	ensure (*l++ == exp(t[0]));
	ensure (*l++ == exp(t[1]));
	ensure (*l++ == exp(t[2]));
}

template<class T>
void test_derivative(void)
{
	T eps = std::numeric_limits<T>::epsilon();

	ensure (derivative([](T x) { return x; })(1) == 1);
	ensure (fabs(derivative((double(*)(double))exp)(1) - exp(1.)) < sqrt(eps));
}

void test_extrapolate(void)
{
	auto H = extrapolate((double(*)(double))exp, 1., (double(*)(double))log);
	ensure (H(0) == exp(0.));
	ensure (H(1) == exp(1.));
	ensure (H(2) == log(2.));
}

template<class T>
void test_integral(void)
{
	ensure (integral([](T x) { return x; })(1) == .5);
	ensure (fabs(integral([](T x) { return x*x; }, 1000)(1)) < 1./1000000);
}

template<class T, class U>
void test_polynomial(void)
{
	U p[3] = {1, 2, 3};
	auto F = polynomial::horner<T,U>(3, p);
	ensure (F(0) == p[0]);
	ensure (F(1) == p[0] + 1*p[1] + 1*p[2]); 
	ensure (F(2) == p[0] + 2*p[1] + 2*2*p[2]); 

	U q[4] = {1, 2, 3, 4};
	auto G = polynomial::taylor<T,U>(4, q);
	ensure (G(0) == q[0]);
	ensure (G(1) == q[0] + q[1] + (1./2)*(q[2] + (1./3)*q[3]));
	ensure (G(2) == q[0] + 2*(q[1] + (1./2)*2*(q[2] + (1./3)*2*q[3])));
}

int
main(void)
{
	try {
		test_apply_iterator<double>();
		test_apply_iterator<float>();
		test_derivative<double>();
		test_extrapolate();
		test_integral<double>();
		test_polynomial<double,double>();
		test_polynomial<float,float>();
		test_polynomial<double,float>();
		test_polynomial<float,double>();
	}
	catch (const std::exception& ex) {
		std::cerr << ex.what() << std::endl;

		return -1;
	}

	return 0;
}