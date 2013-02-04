// holiday.h - Holidays a la Quantlib
// Copyright (c) 2011 KALX, LLC. All rights reserved. No warranty is made.
#pragma once

#include "datetime.h"

namespace datetime {
namespace holiday {

	// http://www.assa.org.au/edm.html#Computer
	inline
	void easter_month_day(int y, int& m, int& d)
	{
		int fd, r, t;    // intermediate results
		int tA, tB, tC, tD, tE;          // table A to E results

		   fd = y / 100;              // first 2 digits of year
		   r = y % 19;             // remainder of year / 19

		//  calculate PFM date
		   t = (fd - 15) / 2 + 202 - 11 * r;
    
		   switch (fd) {
			  case 21: case 24: case 25: case 27: case 28: case 29: case 30: case 31: case 32: case 34: case 35: case 38:
				 t = t - 1;

				  break;
			  case 33: case 36: case 37: case 39: case 40:
				 t = t - 2;

				 break;
		   }
		   t = t % 30;

		   tA = t + 21;
		   if (t == 29) tA = tA - 1;
		   if (t == 28 && r > 10) tA = tA - 1;

		// find the next Sunday
		   tB = (tA - 19) % 7;
    
		   tC = (40 - fd) % 4;
		   if (tC == 3) tC = tC + 1;
		   if (tC > 1) tC = tC + 1;
        
		   t = y % 100;
		   tD = (t + t / 4) % 7;
    
		   tE = ((20 - tB - tC - tD) % 7) + 1;
		   d = tA + tE;

		// return the date
		   if (d > 31) {
			  d = d - 31;
			  m = 4;
		   }
		   else {
			  m = 3;
		   }

		   ensure (DAY_SUN == datetime::date(y,m,d).weekday());
	}

	// standard holidays
	inline datetime::date 
	new_years_day(int year)
	{
		return datetime::date(year, MONTH_JAN, 1).adjust(ROLL_FOLLOWING_BUSINESS);
	}
	inline datetime::date 
	martin_luther_kings_birthday(int year)
	{
		return datetime::date(year, MONTH_JAN, 1).imm(3, DAY_MON);
	}
	inline datetime::date 
	presidents_day(int year)
	{
		return datetime::date(year, MONTH_FEB, 1).imm(3, DAY_MON);
	}
	inline datetime::date
	st_patricks_day(int year)
	{
		return datetime::date(year, MONTH_MAR, 17);
	}
	inline datetime::date 
	good_friday(int y)
	{
		int m, d;

		easter_month_day(y, m, d);

		return datetime::date(y, m, d - 2);
	}
	inline datetime::date 
	easter(int y)
	{
		int m, d;

		easter_month_day(y, m, d);

		return datetime::date(y, m, d);
	}
	inline datetime::date 
	easter_monday(int y)
	{
		int m, d;

		easter_month_day(y, m, d);

		return datetime::date(y, m, d + 1);
	}
	inline datetime::date 
	may_day(int year)
	{
		datetime::date d(year, MONTH_MAY, 1);
		int wday = d.weekday();

		return wday == DAY_SAT ? d.incr(2, UNIT_DAY) : wday == DAY_SUN ? d.incr(1, UNIT_DAY) : d;
	}
	inline datetime::date 
	may_day_bank_holiday(int year)
	{
		return datetime::date(year, MONTH_MAY, 1).imm(1, DAY_MON);
	}
	// last Monday in May
	inline datetime::date 
	memorial_day(int year)
	{
		return datetime::date(year, MONTH_MAY, 25).imm(0, DAY_MON);
	}
	inline datetime::date 
	spring_bank_holiday(int year)
	{
		return datetime::date(year, MONTH_MAY, 25).imm(0, DAY_MON);
	}
	inline datetime::date 
	june_bank_holiday(int year)
	{
		return datetime::date(year, MONTH_JUN, 1).imm(1, DAY_MON);
	}
	inline datetime::date 
	independence_day(int year)
	{
		datetime::date d(year, MONTH_JUL, 4);
		int wday = d.weekday();

		return wday == DAY_SUN ? d.incr(1, UNIT_DAY) : wday == DAY_SAT ? d.incr(-1, UNIT_DAY) : d;
	}
	inline datetime::date 
	summer_bank_holiday(int year)
	{
		return datetime::date(year, MONTH_AUG, 25).imm(0, DAY_MON); //???
	}
	inline datetime::date 
	labor_day(int year)
	{
		return datetime::date(year, MONTH_SEP, 1).imm(1, DAY_MON);
	}
	inline datetime::date 
	columbus_day(int year)
	{
		return datetime::date(year, MONTH_OCT, 1).imm(2, DAY_MON);
	}
	inline datetime::date 
	veterans_day(int year)
	{
		datetime::date d(year, MONTH_NOV, 11);
		int wday = d.weekday();

		return wday == DAY_SUN ? d.incr(1, UNIT_DAY) : wday == DAY_SAT ? d.incr(-1, UNIT_DAY) : d;
	}
	inline datetime::date 
	thanksgiving_day(int year)
	{
		return datetime::date(year, MONTH_NOV, 1).imm(4, DAY_THU);
	}
	inline datetime::date 
	christmas_day(int year)
	{
		datetime::date d(year, MONTH_DEC, 25);
		int wday = d.weekday();

		return wday == DAY_SUN ? d.incr(1, UNIT_DAY) : wday == DAY_SAT ? d.incr(-1, UNIT_DAY) : d;
	}
	inline datetime::date 
	boxing_day(int year)
	{
		datetime::date d(year, MONTH_DEC, 26);
		int wday = d.weekday();

		return wday == DAY_SUN ? d.incr(1, UNIT_DAY) : wday == DAY_SAT ? d.incr(-1, UNIT_DAY) : d;
	}

} // namespace holiday
} // namespace datetime