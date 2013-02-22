// yield_curve_test.cpp
#include "../yield_curve.h"

using namespace curves::pwflat;

template<class T, class F>
void yield_curve_test(void)
{
	T eps = std::numeric_limits<T>::epsilon();
	T t[] = {1, 2, 3, 4};
	// present_value of (0, -1), (1, e), ..., (n, 1+e) is 0 for f(t) = f0.
	F f0 = (F)0.04;
	F e = exp(f0) - 1;
	// generic
	T u[]  = {0, 1, 2, 3, 4};
	F c3[] = {-1, e, e, 1 + e};
	F c4[] = {-1, e, e, e, 1 + e};

	yield_curve<T,F> yc;

	yc.add(1., 1 + e)
	  .add(1., -1., 2., 1 + e)
	  .add(4, u, c3)
	  .add(5, u, c4);

	const forward<T,F>& f = yc.forward();
	for (size_t i = 0; i < yc.forward().size(); ++i)
		ensure (fabs(f[i] - f0) < eps);
}

void curves_yield_curve_test(void)
{
	yield_curve_test<double,double>();
}