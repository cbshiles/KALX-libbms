// row_test.cpp
#include "../range.h"

using namespace range;

template<class T>
void row_test_(void)
{
	T t[6];
	wrap<T> t1(6, t);
	sequence<T>(t1, 1); // 1, 2, ... 6

	row<T> r0(t1, 2), r1; // two columns
	r1 = r0 + 1;
	row<T> r2(r0 + 2);

	ensure (r0[0] == 1);
	ensure (r2[1] == 6);

	ensure (++r0 == r1);
	ensure (r0++ == r1);
	ensure (r0 == r2);

	std::swap(r1, r2);
	ensure (r1[0] == 5);
	ensure (r2[0] == 3);
}

template<class T>
void row_iterator_test_(void)
{
	T t[6];
	wrap<T> w(6, t);
	sequence<T>(w, 1); // 1, 2, ..., 6

	row<T> r(w, 2);
	std::reverse(r.begin(), r.end());
}

void row_test(void)
{
	row_test_<double>();
	row_iterator_test_<float>();
}