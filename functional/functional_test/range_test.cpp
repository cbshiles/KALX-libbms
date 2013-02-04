// range_test.cpp - test range namespace
#include <iostream>
#include "../range.h"

void index_test(void);
void wrap_test(void);
void hold_test(void);
void row_test(void);
/*
using namespace range;
template<class T>
void test_range_take(void)
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
void test_range_drop(void)
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

template<class T>
void test_range_stride(void)
{
	T _t[] = {1,2,3};
	wrap<T> t(3, _t);

	ensure (t[stride(2)(0)] == _t[0]);
	ensure (t[stride(2)(1)] == _t[2]);
}

template<class T>
void test_range_mask(void)
{
	T _t[] = {1,2,3};
	wrap<T> t(3, _t);

	bool m[] = {true, false, true};
	ensure (t[mask(wrap<bool>(3, m))(0)] == _t[0]);
	ensure (t[mask(wrap<bool>(3, m))(1)] == _t[2]);
}

template<class T>
void test_range_grade(void)
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
}

template<class T>
void test_range_wrapper2(void)
{
	T _t[] = {1, 2, 3,
	        4, 5, 6};
	wrapper2<T> a(2, 3, _t);
	wrapper2<T> a2(a), a3;
	a3 = a;

	ensure (a[0][0] == 1);
	ensure (a[1][2] == 6);

	a[1][2] = 7;
	ensure (a[1][2] == 7);

	ensure (a[0] == wrap<T>(3, _t));
	ensure (a[1] == wrap<T>(3, _t + 3));

	wrapper2<T> b(2, wrap<T>(6, _t));
	ensure (b[0] == wrap<T>(3, _t));
	ensure (b[1] == wrap<T>(3, _t + 3));
}

void test_range(void)
{
	test_range_take<double>();
	test_range_drop<float>();
	test_range_stride<size_t>();
	test_range_mask<long>();
	test_range_grade<int>();
}
*/

int
main(int ac, const char* av[])
{
	try {
		index_test();
		wrap_test();
		hold_test();
		row_test();
//		test_range();
	}
	catch (const std::exception& ex) {
		std::cerr << ex.what() << std::endl;

		return -1;
	}

	return 0;
}