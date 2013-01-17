// tnumerical.cpp - test numerical routines
// Copyright (c) 2013 KALX, LLC. All rights reserved. No warranty is made.

#include <cmath>
#include <iostream>
#define ENSURE_BREAK
#include "numerical.h"

using namespace numerical;


void test_srng_seed(size_t N = 1000)
{
	srng r1;
	srng r2(true);
	ensure (r1 == r2);
	ensure (!(r1 < r2));
	ensure (!(r1 != r2));
	ensure (r1 <= r2);
	ensure (!(r1 > r2));
	ensure (r1 >= r2);

	for (size_t i = 0; i < N; ++i)
		ensure (r1.uint() == r2.uint());

	srng r3(12345678, 87654321);
	ensure (r3 != r2);
	r2 = r3;
	ensure (r3 == r2);

	srng r4(r3);
	ensure (r4 == r3);
}

void test_srng(size_t N = 10000)
{
	test_srng_seed();

	srng rng(true);

	double u, min = 1, max = 0;
	double hist[10] = {0,0,0,0,0,0,0,0,0,0};
	for (size_t i = 0; i < N; ++i) {
		u = rng.real();
		min = u < min ? u : min;
		max = u > max ? u : max;
		hist[static_cast<int>(u*10)]++;
	}

	// test uniform to 3 standard deviations
	for (size_t i = 0; i < 10; ++i)
		ensure (std::abs(hist[i] - N/10) <= 3*sqrt(N/10.));

	ensure (min < 2.*sqrt(1./N));
	ensure (1 - max < 2.*sqrt(1./N));
	
}

#ifdef __GNUC__
#define _nextafter nextafter
#endif
void test_ulp(size_t N = 10000)
{
	srng rng;

	for (size_t i = 0; i < N; ++i) {
		double x = 1/(1 - rng.real());
		ensure (1 == ulp(x, _nextafter(x, x - 1)));
	}
}

int main(void)
{
	try {
		test_srng();
		test_ulp();
	}
	catch (const std::exception& ex) {
		std::cerr << ex.what() << std::endl;

		return -1;
	}

	return 0;
}
