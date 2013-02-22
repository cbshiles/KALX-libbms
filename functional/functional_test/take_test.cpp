// take_test.cpp
#include "../range.h"

using namespace range;

template<class T>
void take_test_(void)
{
	T _t[] = {1,2,3};
	wrap<T> t(3, _t);

	ensure (take(2,t)[0] == _t[0]);
	ensure (take(2,t)[1] == _t[1]);
	ensure (take(2,t)[2] == _t[0]);
	ensure (take(2,t)[-1] == _t[1]);

	ensure (take(-2,t)[0] == _t[1]);
	ensure (take(-2,t)[1] == _t[2]);
	ensure (take(-2,t)[2] == _t[1]);
	ensure (take(-2,t)[-1] == _t[2]);
}

template<class T>
void drop_test_(void)
{
	T _t[] = {1,2,3};
	wrap<T> t(3, _t);

	ensure (drop(1,t)[0] == _t[1]);
	ensure (drop(1,t)[1] == _t[2]);
	ensure (drop(1,t)[2] == _t[1]);
	ensure (drop(1,t)[-1] == _t[2]);

	ensure (drop(-1,t)[0] == _t[0]);
	ensure (drop(-1,t)[1] == _t[1]);
	ensure (drop(-1,t)[2] == _t[0]);
	ensure (drop(-1,t)[-1] == _t[1]);
}

void take_test(void)
{
	take_test_<double>();
	drop_test_<double>();
}