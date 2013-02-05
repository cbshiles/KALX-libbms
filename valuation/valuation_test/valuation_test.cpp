// valuation_test.cpp
#include <iostream>
#include "../gbm.h"

using namespace valuation;

int main(void)
{
	put<double> p(1,2);
	bms<double> m(3,4);

	double v = value(p, m);

	return 0;
}