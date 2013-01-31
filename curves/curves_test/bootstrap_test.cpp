// bootstrap_test.cpp
#include <vector>
#include "../bootstrap.h"

using namespace curves::pwflat;

#define dimof(x) (sizeof(x)/sizeof(*x))

template<class T, class U>
void forward_constructors(void)
{
	forward<T,U> F;
	ensure (F.size() == 0);
	ensure (F(-1) == 0);
	ensure (F(1) == 0);

	F.extrapolate(2);
	ensure (F(-1) == 2);
	ensure (F(1) == 2);

	forward<T,U> F2(F);
	ensure (F2(-1) == 2);
	ensure (F2(1) == 2);

	F2.extrapolate(3);
	F = F2;
	ensure (F(-1) == 3);
	ensure (F(1) == 3);

	T t[] = {1, 2, 3};
	U f[] = {.1, .2, .3};
	forward<T,U> F3(3, t, f);
	ensure (F3.size() == 3);

	forward<T,U> F4(F3);
	ensure (F4.size() == 3);

	F3 = F4;
	ensure (F3.size() == 3);
}

template<class T, class U>
void forward_extrapolate(void)
{
	T t[] = {1,2,3};
	U f[] = {.1,.2,.3};
	size_t n = dimof(t);

	forward<T,U> F(n, t, f);
	F.extrapolate(.4);
	ensure (F(4) == .4);

	F = F.extrapolate(.5);
	ensure (F(4) == .5);
}

template<class T, class U>
void forward_value(void)
{
	T t[] = {1,2,3};
	U f[] = {.1,.2,.3};
	size_t n = dimof(t);
	ensure (n == dimof(f));

	forward<T,U> F(n, t, f);

	ensure (f[0] == F(-1));
	ensure (f[0] == F(1));
	ensure (f[1] == F(1.1));
	ensure (f[0] == F(0.9));
	ensure (f[1] == F(2));
	ensure (f[2] == F(3));
	ensure (1.23 == F.extrapolate(1.23)(4));

	ensure (F.size() == n);

	for (size_t i = 0; i < n; ++i) {
		ensure (F(t[i]) == f[i]);
	}
}

template<class T, class U>
void forward_integral(void)
{
	U eps = std::numeric_limits<U>::epsilon();

	T t[] = {1,2,3};
	U f[] = {.1,.2,.3};
	size_t n = dimof(t);
	ensure (n == dimof(f));

	forward<T,U> F(n, t, f);

	ensure (f[0] == F.integral(1));
	ensure (f[0] + f[1]*.5 == F.integral(1.5));
	ensure (f[0] + f[1] == F.integral(2));
	ensure (fabs(f[0] + f[1] + f[2]*.5 - F.integral(2.5)) < eps);
	ensure (fabs(f[0] + f[1] + f[2] - F.integral(3)) < eps);
	ensure (fabs(f[0] + f[1] + f[2] - F.integral(4)) < eps);
	F.extrapolate(.4);
	ensure (fabs(f[0] + f[1] + f[2] + .4*.5 - F.integral(3.5)) < eps);
}

template<class T, class U>
void bootstrap_test(void)
{
	T eps = std::numeric_limits<T>::epsilon();
	T t[] = {1, 2, 3, 4};
//	size_t n = sizeof(t)/sizeof(*t);

	std::vector<U> f;

	// present_value of (0, -1), (1, e), ..., (n, 1+e) is 0 for f(t) = f0.
	U f0 = (U)0.04;
	U e = exp(f0) - 1;

	// cd with empty forward curve
	f.push_back(forward<T,U>().bootstrap1(1., 1 + e));
	ensure (fabs(f.back() - f0) < eps);

	// fra
	f.push_back(forward<T,U>(1, t, &f[0]).bootstrap2(1., -1., 2., 1 + e));
	ensure (fabs(f.back() - f0) < eps);

	// generic
	T u[]  = {0, 1, 2, 3, 4};
	T c3[] = {-1, e, e, 1 + e};
	// initial guess 0.02
	f.push_back(forward<T,U>(2, t, &f[0], (U)0.02).bootstrap(4, u, c3)); 
	ensure (fabs(f.back() - f0) < eps);

	T c4[] = {-1, e, e, e, 1 + e};
	f.push_back(forward<T>(3, t, &f[0], (U).02).bootstrap(5, u, c4)); 
	ensure (fabs(f.back() - f0) < eps);
}

void curves_bootstrap_test(void)
{
	forward_constructors<double,double>();
	forward_extrapolate<double,double>();
	forward_value<double,double>();
	forward_integral<double,double>();
	forward<double,double>();
	bootstrap_test<double,double>();
}