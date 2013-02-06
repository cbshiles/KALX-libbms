// index_test.cpp
#include "../range.h"

using namespace range;


template<class T>
void index_test_(void)
{
	T _t[] = {1,2,3};
	wrap<T> t(3, _t);

	counter n(1);
	ensure (t[0] == n);
	ensure (t[1] == n);
	ensure (t[2] == n);

	ensure (t[0] == _t[0]);
	ensure (t[-1] == _t[2]);
	ensure (t[3] == _t[0]);
	ensure (t[-3] == _t[0]);
	ensure (t[100] == _t[1]);

	t[0] = 4;
	ensure (t[0] == 4);
	t[-2] = 5;
	ensure (t[1] == 5);
	t[-1-3] = 6;
	ensure (t[2] == 6);

	std::transform(t.begin(), t.end(), t.begin(), [](T t) -> T { static counter n(2); return 2*n; });
	ensure (_t[0] == 4);
	ensure (t[1] == 6);
	ensure (t[2] == 8);

	sequence<T>(t, 4, 2);
	ensure (t[0] == 4);
	ensure (_t[1] == 6);
	ensure (t[2] == 8);

	hold<T> u = sequence<T>(4, 8, 2);
	ensure (u == t);
}

void index_test(void)
{
	index_test_<double>();
}