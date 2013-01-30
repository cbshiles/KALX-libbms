// polynomial_test.cpp
#include "../../numerical/ulp.h"
#include "../polynomial.h"

//using namespace std;
using namespace numerical;
using namespace curves::polynomial;

template<class T, class U>
void curves_polynomial_(void)
{
	U p[3] = {1, 2, 3};
	auto F = horner<T,U>(3, p);
	ensure (F(0) == p[0]);
	ensure (F(1) == p[0] + 1*p[1] + 1*p[2]); 
	ensure (F(2) == p[0] + 2*p[1] + 2*2*p[2]); 

	U q[4] = {1, 2, 3, 4};
	U one(1), two(2), three(3);
	auto G = taylor<T,U>(4, q);
	ensure (G(0) == q[0]);
	U gx;
	gx = q[0] + one*(q[1] + (one/two)*(q[2] + (one/three)*q[3]));
	ensure (abs(ulp(G(1), gx)) <= 1);
	gx = q[0] + two*(q[1] + (two/two)*(q[2] + (two/three)*q[3]));
	ensure (abs(ulp(G(2), gx)) <= 1);
}

void curves_polynomial_test(void)
{
	curves_polynomial_<float,float>();
	curves_polynomial_<double,double>();
}


