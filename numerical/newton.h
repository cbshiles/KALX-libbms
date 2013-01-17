// newton.h - self containted 1d root finding using the Newton method
// Copyright (c) 2013 KALX, LLC. All rights reserved.
#pragma once
#include <limits>

namespace numerical {
namespace root1d {

	template<class T, class F, class dF>
	inline T newton(T x, const F& f, const dF& df, size_t iter = -1)
	{
		T fx = f(x);
		T dfx = df(x);

		while (iter-- && 1 + fx != 1 && 1 + dfx != 1) {
			T x_ = x - fx/dfx;
			if (x_ == x)
				break;
			x = x_;
			fx = f(x);
			dfx = df(x);
		} 

		return (iter && 1 + dfx != 1) ? x : std::numeric_limits<T>::quiet_NaN();
	}

} // namespace root1d
} // namespace numerical

#ifdef _DEBUG

template<class T>
void test_numerical_root1d_newton_(size_t N = 10000)
{
	srng rng;
	T eps = std::numeric_limits<T>::epsilon();

	for (size_t i = 0; i < N; ++i) {
		T a = static_cast<T>(rng.real());
		T b = static_cast<T>(rng.real());
		T c = static_cast<T>(rng.real());
		if (b > c)
			std::swap(b, c);
//		cout << "a = " << a << " b = " << b << " c = " << c << endl;

		auto f = [a,b,c](T x) { return a*(x - b)*(x - c); };
		auto df = [a,b,c](T x) { return a*((x - b) + (x - c)); };

		T r;
		T three = 3;

		r = root1d::newton((b + c)/three, f, df);
//		cout << b - r << endl;
		ensure (fabs(b - r)*a*(c - b) <= eps);
		r = root1d::newton((b + c)/(three/2), f, df);
//		cout << c - r << endl;
		ensure (fabs(c - r)*a*(c - b) <= eps);

	}
}

struct test_numerical_root1d_newton {
	test_numerical_root1d_newton(void)
	{
		test_numerical_root1d_newton_<double>();
		test_numerical_root1d_newton_<float>();
	}
};

static test_numerical_root1d_newton test_numerical_root1d_newton0;

#endif // _DEBUG