// hold_test.cpp
#include "../range.h"

using namespace range;

template<class T>
void hold_test_(void)
{
	hold<T> h0;
	ensure (h0.size() == 0);
	ensure (!h0.begin());

	T t[] = {1, 2, 3};
	
	hold<T> h1(3, t);
	ensure (h1.size() == 3);
	ensure (h1[0] == 1);
	ensure (h1[1] == 2);
	ensure (h1[2] == 3);

	hold<T> h2(h1);
	ensure (h2 == h1);

	h0 = h1;
	ensure (h0 == h2);
}

void hold_test(void)
{
	hold_test_<double>();
}