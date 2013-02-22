// grade_test.cpp
#include "../sort.h"

using namespace range;

template<class T>
void grade_test_(void)
{
	T _t[] = {3,1,4,2};
	wrap<T> t(4, _t);

	hold<size_t> i = grade<T>(t);
	for (T j = 0; j < 4; ++j)
		ensure (t[i[j]] == j + 1);

	i = grade<T>(t, 2);
	ensure (t[i[0]] == 1);
	ensure (t[i[1]] == 2);

	i = grade<T>(t, -1);
	ensure (t[i[0]] == 4);
	ensure (t[i[1]] == 3);
	ensure (t[i[2]] == 2);
	ensure (t[i[3]] == 1);

	i = grade<T>(t, -2);
	ensure (t[i[0]] == 4);
	ensure (t[i[1]] == 3);

	size_t _v[] = {1,0};
	wrap<size_t> v(2, _v);
	i = grade<T>(t, 2, v, -1);
	ensure (i.size() == 2);
	ensure (i[0] == 1);
	ensure (i[1] == 0);
}

void grade_test(void)
{
	grade_test_<int>();
}
