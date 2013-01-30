// numerical_test.cpp : main project file.
#include <iostream>

void numerical_root1d_newton_test(void);

int main(void)
{
	try {
		numerical_root1d_newton_test();
	}
	catch (const std::exception& ex){
		std::cerr << ex.what() << std::endl;

		return -1;
	}

    return 0;
}
