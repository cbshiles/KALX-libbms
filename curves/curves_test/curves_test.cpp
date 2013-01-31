// curves_test.cpp - test curves
#include <iostream>

void curves_basis_spline_test(void);
void curves_bootstrap_test(void);
void curves_polynomial_test(void);
void curves_yield_curve_test(void);

int
main()
{
	try {
		curves_basis_spline_test();
		curves_bootstrap_test();
		curves_polynomial_test();
		curves_yield_curve_test();
	}
	catch (const std::exception& ex) {
		std::cerr << ex.what() << std::endl;

		return -1;
	}

	return 0;
}
