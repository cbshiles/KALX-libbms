// row_iterator_test.cpp - test row_iterator
#include "../row_iterator.h"

using namespace range;


template<class T>
void row_iterator_test_(void)
{
	hold<T> s = sequence<T>(1,6);

	row<T> r0(s, 2);
	row<T> r1(r0 + 1);
	row<T> r2;
	r2 = r0 + 2;

	ensure (r0[0] == 1 && r0[1] == 2);
	ensure (r1[0] == 3 && r1[1] == 4);
	ensure (r2[0] == 5 && r2[1] == 6);

	std::reverse(r0.begin(), r0.end());
	ensure (r2[0] == 1 && r2[1] == 2);
	ensure (r1[0] == 3 && r1[1] == 4);
	ensure (r0[0] == 5 && r0[1] == 6);

	std::reverse(r0.begin(), r0.end());

	wrap<row<double>> r(2, &r0);
	drop(2, r);
}

void row_iterator_test(void)
{
	row_iterator_test_<double>();
}