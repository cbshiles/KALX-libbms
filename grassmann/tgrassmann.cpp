// tgrassmann.cpp - test Grassmann algebra
#include <iostream>
#include <random>
#include "grassmann.h"

using namespace std;
using namespace grassmann;

std::default_random_engine rng;//(std::random_device());

template<class U>
inline bool equivalent(const U& a, const U& b)
{
	return !(a < b) && !(b < a);
}

template<size_t N>
void test_extensor(size_t n = 1000)
{
	std::uniform_int_distribution<size_t> uid(0,-1 + (1<<N));
	std::variate_generator<default_random_engine,uniform_int_distribution<size_t>> u(rng, uid);

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

	for (size_t x = 0; x < n; ++x) {
		unsigned long long i, j, k;
		i = u();
		j = u();
		k = u();
		extensor<N> Pi(i), Pj(j), Pk(k);
		ensure (!(Pi<Pi));
		ensure (!(Pj<Pj));
		ensure (!(Pk<Pk));
		ensure (Pi < Pj ? !(Pj < Pi) : true);
		ensure ((Pi < Pj && Pj < Pk) ? Pi < Pk : true);
		ensure ((equivalent(Pi, Pj) && equivalent(Pj, Pk)) ? equivalent(Pi,Pk) : true);
	}
}

template<size_t N>
void test_element(void)
{
	element<N> A;

	A += element<N>(extensor<N>(1));
	A += A;
	ensure (A[extensor<N>(1)] == 2);
	A *= 3;
	ensure (A[extensor<N>(1)] == 6);

	point<0,N> P0;
	ensure (P0.size() == 1);
	ensure (P0[extensor<N>(1)] == 1);
	ensure (P0[extensor<N>(2)] == 0);

	A += point<1,N>()*point<2,N>();
	ensure (A.size() == 2);
}

#define P(i) point<i,3>()
#define E(i) (P(i) - P(0))
void test_convex(void)
{
	auto X = [](double x) { return P(0) + x*E(1) + (x > 100 ? x - 100 : 0)*E(2); };

	element<3> X90(X(90)), X100(X(100)), X110(X(110));
	element<3> x = P(0) + 100.*E(1) + 4.*E(2);
	element<3> Omega(X90*X100*X110);

	ensure ((x*X100*X110)/Omega > 0);
	ensure ((X90*x*X110)/Omega > 0);
	ensure ((X90*X100*x)/Omega > 0);
}

int
main(void)
{
	test_extensor<3>();
	test_element<4>();

	test_convex();

	return 0;
}