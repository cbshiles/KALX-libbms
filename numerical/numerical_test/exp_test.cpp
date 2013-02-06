// exp_test.cpp - test exponential function
#include <cmath>
#include "../../include/timer.h"
#include "../exp.h"
#include "../ulp.h"

template<class T>
void exp_test_(void)
{
	utility::timer t;

	t.start();
	T e = numerical::exp<T>(1);
	t.stop();
	double dt = t.elapsed();

	t.start();
	T e0 = ::exp((T)1);
	t.stop();
	dt = t.elapsed();

	for (T t = -10; t <= 10; t += 1) {
		T e0 = ::exp(t);
		T e = numerical::exp(t);
		numerical::ulp_traits<T>::integer i = numerical::ulp(e0, e);
		ensure (abs(i) < 1e300);
	}
}

void exp_test(void)
{
	exp_test_<double>();
}