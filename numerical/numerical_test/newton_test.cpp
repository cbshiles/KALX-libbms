// newton_test.cpp
#include "../srng.h"
#include "../newton.h"

using namespace numerical;

template<class T>
void root1d_newton_test_(size_t N = 10000)
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

void
root1d_newton_test(void)
{
	root1d_newton_test_<double>();
	root1d_newton_test_<float>();
}
