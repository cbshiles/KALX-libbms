// wrap_test.cpp
#include "../range.h"

using namespace range;

template<class T>
void wrap_test_(void)
{
	wrap<T> t0;
	ensure (t0.size() == 0);
	ensure (!t0.begin());

	T t[] = {1,2,3};
	wrap<T> t1(3, t);
	ensure (t1.size() == 3);
	ensure (t1[0] == 1);
	ensure (t1[1] == 2);
	ensure (t1[2] == 3);

	t0 = t1;
	ensure (t0.size() == 3);
	ensure (t0[0] == 1);
	ensure (t0[1] == 2);
	ensure (t0[2] == 3);

	wrap<T> t2(t0);
	ensure (t2 == t0);

	t2[1] = 4;
	ensure (t[1] == 4); // aliased

	wrap<const T> t3(3, t);
	// t3[1] = 5; fails to compile
}

void wrap_test(void)
{
	wrap_test_<double>();
	wrap_test_<float>();
	wrap_test_<int>();
}
