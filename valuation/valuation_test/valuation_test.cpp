// valuation_test.cpp
#include <iostream>
#include "../gbm.h"

using namespace valuation;

int main(void)
{
	double forward(100), strike(100), volatility(0.2), expiration(0.25);

	double v = value(put<>(strike, expiration), bms<>(forward,volatility));

	return 0;
}