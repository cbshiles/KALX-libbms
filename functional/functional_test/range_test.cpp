// range_test.cpp - test range namespace
#include <iostream>
#include "../range.h"

void index_test(void);
void wrap_test(void);
void hold_test(void);
void row_test(void);
void take_test(void);
void grade_test(void);
void mask_test(void);
void row_iterator_test(void);

int
main(int ac, const char* av[])
{
	try {
		index_test();
		wrap_test();
		hold_test();
		row_test();
		take_test();
		grade_test();
		mask_test();
		row_iterator_test();
	}
	catch (const std::exception& ex) {
		std::cerr << ex.what() << std::endl;

		return -1;
	}

	return 0;
}