// cmegroup.h - Chicago Mercantile Exchange Group related conventions.
// Copyright (c) 2003-2011 KALX, LLC. All rights reserved. No warranty is made.
#pragma once

#include <ctype.h>

namespace cmegroup {

	static const int call_month_code_table[] = { 0,
		'A', 'B', 'C',
		'D', 'E', 'F',
		'G', 'H', 'I',
		'J', 'K', 'L'
	};
	static const int put_month_code_table[] = { 0,
		'M', 'N', 'O',
		'P', 'Q', 'R',
		'S', 'T', 'U',
		'V', 'W', 'X'
	};
	static const int option_code_month_table[] = {
		 1,  2,  3,  4,  5,  6,  7,  8,  9,  10,  11,  12,
		-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12,
	};
	static const int futures_month_code_table[] = { 0,
		'F', 'G', 'H',
		'J', 'K', 'M',
		'N', 'Q', 'U',
		'V', 'X', 'Z'
	};
	static const int futures_code_month_table[] = {
		0, 0, 0, 0, 0, 1, 2, 3,  0, 4,  5, 0,  6,
		7, 0, 0, 8, 0, 0, 0, 9, 10, 0, 11, 0, 12
	};

	/* map month (JAN = 1) to letter code for equity option */
	inline int
	option_month_code(int month)
	{
		if (month > 12)
			month = 0;
		else if (month < -12)
			month = 0;

		return month > 0 ? call_month_code_table[month] : put_month_code_table[-month];
	}
	/* map equity option letter code to month */
	inline int
	option_code_month(int code)
	{
		int i = toupper(code) - 'A';

		if (i < 0 || i >= sizeof(option_code_month_table)/sizeof(int))
			return 0;

		return option_code_month_table[i];
	}

	/* Return month code ASCII character for month on a futures */
	inline int
	futures_month_code(int month)
	{
		if (month < 1 || month > 12)
			return 0;

		return futures_month_code_table[month];
	}
	/* Return month corresponding to ASCII character for month of a futures */
	inline int
	futures_code_month(int code)
	{
		int i = toupper(code) - 'A';

		if (i < 0 || i >= sizeof(futures_code_month_table)/sizeof(int))
			return 0;

		return futures_code_month_table[i];
	}

} // namespace cmegroup
