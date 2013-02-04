// calendar.h - Holiday calendars.
// Copyright (c) 2003-2011 KALX, LLC. All rights reserved. No warranty is made.
#pragma once

#include "holiday.h"

#define HOLIDAY_DECL int y, m, d; t.localtime(&y, &m, &d); datetime::date t0(y, m, d);
#define HOLIDAY(h) (t0 == holiday::h(y))
#define HOLIDAY_DATE(y,m,d) (t0 == datetime::date(y,m,d))

namespace datetime {
namespace calendar {

// template for adding calendar data
// US Treasuries (SIFMA recommended closings) 
inline bool
BMA(const datetime::date& t)
{
	HOLIDAY_DECL
	
	return HOLIDAY(new_years_day)
		|| HOLIDAY(martin_luther_kings_birthday)
		|| HOLIDAY(presidents_day)
		|| HOLIDAY(good_friday)
		|| HOLIDAY(memorial_day)
		|| HOLIDAY(independence_day)
		|| HOLIDAY(labor_day)
		|| HOLIDAY(columbus_day)
		|| HOLIDAY(veterans_day)
		|| HOLIDAY(thanksgiving_day)
		|| HOLIDAY(christmas_day)
		;
}

// http://www.cmegroup.com/tools-information/holiday-calendar/index.html
// Chicago Mercantile Exch. trade date holidays (Globex) 
inline bool 
CMM(const datetime::date& t)
{
	HOLIDAY_DECL

	return HOLIDAY(new_years_day)
		|| HOLIDAY(martin_luther_kings_birthday)
		|| HOLIDAY(presidents_day)
		|| HOLIDAY(good_friday)
		|| HOLIDAY(memorial_day)
		|| HOLIDAY(independence_day)
		|| HOLIDAY(labor_day)
		|| HOLIDAY(columbus_day)
		|| HOLIDAY(veterans_day)
		|| HOLIDAY(thanksgiving_day)
		|| HOLIDAY(christmas_day)
		;
}

// bbalibor currency calendars
inline bool 
EUR(const datetime::date& t)
{
	HOLIDAY_DECL

	return HOLIDAY(new_years_day)
		|| HOLIDAY(good_friday)
		|| HOLIDAY(easter_monday)
//		|| HOLIDAY(may_day) // overnight only
		|| HOLIDAY(christmas_day)
		|| HOLIDAY(boxing_day)
		;
}

inline bool
GBP(const datetime::date& t)
{
	HOLIDAY_DECL

	return HOLIDAY(new_years_day)
		|| HOLIDAY(good_friday)
		|| HOLIDAY(easter_monday)
		|| HOLIDAY(may_day_bank_holiday)
		|| HOLIDAY(spring_bank_holiday)
		|| HOLIDAY(summer_bank_holiday)
		|| HOLIDAY(christmas_day)
		|| HOLIDAY(boxing_day)
		;
}
// London bank holidays	
inline bool
LnB(const datetime::date& t)
{
	HOLIDAY_DECL

	return HOLIDAY(new_years_day)
		|| HOLIDAY(st_patricks_day)
		|| HOLIDAY(good_friday)
		|| HOLIDAY(easter_monday)
		|| HOLIDAY(may_day_bank_holiday)
		|| HOLIDAY(spring_bank_holiday)
		|| HOLIDAY(summer_bank_holiday)
		|| HOLIDAY(christmas_day)
		|| HOLIDAY(boxing_day)
		;
}

// http://www.nerc.com
inline bool 
NER(const datetime::date& t)
{
	HOLIDAY_DECL

	return HOLIDAY(new_years_day)
		|| HOLIDAY(memorial_day)
		|| HOLIDAY(independence_day)
		|| HOLIDAY(labor_day)
		|| HOLIDAY(thanksgiving_day)
		|| HOLIDAY(christmas_day)
		;
}

// New York bank = QuantLib Settlement calendar.
inline bool
NYB(const datetime::date& t)
{
	HOLIDAY_DECL

	return HOLIDAY(new_years_day)
		|| HOLIDAY(martin_luther_kings_birthday)
		|| HOLIDAY(presidents_day)
		|| HOLIDAY(memorial_day)
		|| HOLIDAY(independence_day)
		|| HOLIDAY(labor_day)
		|| HOLIDAY(columbus_day)
		|| HOLIDAY(veterans_day)
		|| HOLIDAY(thanksgiving_day)
		|| HOLIDAY(christmas_day)
		;
}

// New York Stock Exchange trading holidays
inline bool
NYS(const datetime::date& t)
{
	HOLIDAY_DECL

	return 	HOLIDAY(new_years_day)
		|| (y == 2007 && HOLIDAY_DATE(2007, MONTH_JAN, 2))  // Ford
		|| (y >= 1998 && HOLIDAY(martin_luther_kings_birthday))
		|| HOLIDAY(presidents_day)
		|| HOLIDAY(good_friday)
		|| HOLIDAY(memorial_day)
		|| (y == 2004 && HOLIDAY_DATE(2004, MONTH_JUN, 11)) // Reagan
		|| HOLIDAY(independence_day)
		|| HOLIDAY(labor_day)
		|| (y == 2001 && (
			   HOLIDAY_DATE(2001, MONTH_SEP, 11)
			|| HOLIDAY_DATE(2001, MONTH_SEP, 12)
			|| HOLIDAY_DATE(2001, MONTH_SEP, 13)
			|| HOLIDAY_DATE(2001, MONTH_SEP, 14)
		   ))
		|| HOLIDAY(thanksgiving_day)
		|| HOLIDAY(christmas_day)
		;
}


} // namespace calendar
} // namespace datetime

#undef HOLIDAY_DECL
#undef HOLIDAY
#undef HOLIDAY_DATE