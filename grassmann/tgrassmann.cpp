// tgrassmann.cpp - test Grassmann algebra
#include <iostream>
#include "grassmann.h"

using namespace std;
using namespace grassmann;

template<size_t N>
void test_extensor(void)
{
	extensor<N> P0(1), P1(2), P2(4), P(7);

	ensure (sign(0, P0) == 0);
	ensure (sign(0, P1) == 1);
	ensure (sign(1, P0) == -1);

	ensure (sign(P0, P1) == 1);
	ensure (sign(P1, P0) == -1);
	ensure (sign(P2, P2) == 0);
	ensure (sign(P0, P) == 0);
	ensure (sign(P1, P) == 0);
	ensure (sign(P2, P) == 0);
}

template<size_t N>
void test_element(void)
{
	element<N> A;

	A += element<N>(extensor<N>(1));
	A += A;
	ensure (A[extensor<N>(1)] == 2);

	point<0,N> P0;
	ensure (P0.size() == 1);
	ensure (P0[extensor<N>(1)] == 1);
	ensure (P0[extensor<N>(2)] == 0);
}

int
main(void)
{
	test_extensor<3>();
	test_element<4>();

	return 0;
}