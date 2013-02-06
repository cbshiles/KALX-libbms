// row_iterator_test.cpp - test row_iterator
#include "../row_iterator.h"

using namespace range;


template<class T>
void row_iterator_test(void)
{
	sequence<T> s(1,6);

	row<T> r0(&s[0], 2), r1(&s[2], 2), r2(&s[4], 2);

	ensure (r0 == sequence<T>(0,1));
}