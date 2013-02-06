// row_test.cpp
#include "../row_iterator.h"

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
	row_iterator<T> ir(r);
	ensure (*ir == row<T>(w,2));
	++ir;
	ensure (*ir == row<T>(w,2) + 1);
	ir++;
	ensure (*ir == row<T>(w,2) + 2);

	ensure (r.size() == 2);
	ensure (r[0] == t[0]);
	ensure (r[1] == t[1]);
	ir = row_iterator<T>(r);
	ensure (ir == r.begin());
	ir += 3;
	ensure (ir == r.end());
	
	std::reverse(r.begin(), r.end());
	ensure (t[0] == 5);
	ensure (t[1] == 6);
	ensure (t[2] == 3);
	ensure (t[3] == 4);
	ensure (t[4] == 1);
	ensure (t[5] == 2);
}

void row_test(void)
{
	row_test_<double>();
	row_iterator_test_<float>();
}