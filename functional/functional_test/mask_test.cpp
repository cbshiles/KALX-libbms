// mask_test.cpp
#include "../range.h"

using namespace range;

template<class T>
void mask_test_(void)
{
	T _t[] = {1,2,3};
	wrap<T> t(3, _t);

	bool m[] = {true, false, true};
	mask(t, wrap<bool>(3, m));
	ensure (t.size() == 2);
	ensure (t[0] = 1 && t[1] == 3);
}

void mask_test(void)
{
	mask_test_<double>();
}