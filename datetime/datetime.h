// datetime.h - date and time routines
// Copyright (c) 2011 KALX, LLC. All rights reserved. No warranty is made.
#pragma once
#pragma warning(disable: 4127)

#include <cmath>
#include <ctime>
#include <algorithm>
#include <functional>
#include <limits>
#include <vector>
#include "dt.h"

namespace datetime {

typedef enum time_unit {
	UNIT_SECOND = 1, UNIT_SECONDS = 1,
	UNIT_MINUTE = 2, UNIT_MINUTES = 2,
	UNIT_HOUR   = 3, UNIT_HOURS   = 3,
	UNIT_DAY    = 4, UNIT_DAYS    = 4,
	UNIT_WEEK   = 5, UNIT_WEEKS   = 5,
	UNIT_MONTH  = 6, UNIT_MONTHS  = 6,
	UNIT_YEAR   = 7, UNIT_YEARS   = 7,
	UNIT_BUSINESS_DAY  = 9,
	UNIT_BUSINESS_DAYS = 9,
	UNIT_END_OF_MONTH = 10,
	UNIT_FIRST_OF_MONTH,
	UNIT_MAX
};

typedef enum day_of_week {
	DAY_SUN = 0,
	DAY_MON = 1,
	DAY_TUE = 2,
	DAY_WED = 3,
	DAY_THU = 4,
	DAY_FRI = 5,
	DAY_SAT = 6,
	DAY_MAX
};

typedef enum month_of_year {
	MONTH_JAN = 1,
	MONTH_FEB = 2,
	MONTH_MAR = 3,
	MONTH_APR = 4,
	MONTH_MAY = 5,
	MONTH_JUN = 6,
	MONTH_JUL = 7,
	MONTH_AUG = 8,
	MONTH_SEP = 9,
	MONTH_OCT = 10,
	MONTH_NOV = 11,
	MONTH_DEC = 12,
	MONTH_MAX
};

typedef enum day_count_basis {
	DCB_ACTUAL_YEARS = 0,
	DCB_30U_360,
	DCB_30E_360,
	DCB_ACTUAL_360,
	DCB_ACTUAL_365,
	DCB_ACTUAL_ACTUAL_ISDA,
	DCB_ACTUAL_ACTUAL_ICMA,
	DCB_MAX
};

typedef enum payment_frequency {
	FREQ_NO_FREQUENCY = 0, FREQ_NONE = 0,
	FREQ_ANNUALLY     = 1,
	FREQ_SEMIANNUALLY = 2,
	FREQ_QUARTERLY    = 4,
	FREQ_MONTHLY      = 12,
	FREQ_BIWEEKLY     = 26,
	FREQ_WEEKLY       = 52,
	FREQ_MAX
};

typedef enum roll_convention {
	ROLL_NONE = 0, 
	ROLL_FOLLOWING_BUSINESS, 
	ROLL_PREVIOUS_BUSINESS,
	ROLL_MODIFIED_FOLLOWING,
	ROLL_MODIFIED_PREVIOUS,
	ROLL_MAX
};

class date;
typedef std::function<bool(const date&)> holiday_calendar;
#define CALENDAR_NONE [](const date&) { return false; }

class date {
	// returns true if date is a holiday
private:
	// UTC
	time_t t_;

protected:
	date& incr(int days)
	{
		int y, m, d, h, n, s;

		localtime(&y, &m, &d, &h, &n, &s);
		d += days;
		maketime(y, m, d, h, n, s);

		return *this;
	}

public:
	// invalid date
	date()
		: t_(-1)
	{
	}
	explicit date(time_t t)
		: t_(t)
	{
		ensure(is_valid());
	}
	// Excel time
	explicit date(double t)
	{
		t_ = excel2time_t(t);
		ensure (is_valid());
	}

	date(int y, int m, int d, int h = 0, int n = 0, int s = 0)
	{
		maketime(y, m, d, h, n, s);
		ensure (is_valid());
	}

	date(const struct tm& t)
	{
		maketime(t.tm_year + 1900, t.tm_mon + 1, t.tm_mday,
			t.tm_hour, t.tm_min, t.tm_sec);
		ensure (is_valid());
	}

	date(const date& d)
		: t_(d.t_)
    {
    }
    date& operator=(const date& d)
    {
        if (this != &d)
            t_ = d.t_;

        return *this;
    }
	~date()
	{ }

	bool operator==(const date& d) const
	{
		return t_ == d.t_;
	}
	bool operator<(const date& d) const
	{
		return t_ < d.t_;
	}

	bool is_valid() const
	{
		return t_ != -1;
	}

	time_t time() const
	{
		return t_;
	}
	double excel() const
	{
		return time_t2excel(t_); 
	}

	// break out wall clock time
	void localtime(int* py = 0, int* pm = 0, int* pd = 0, 
				int* ph = 0, int* pn = 0, int* ps = 0,
				int* pwday = 0, int* pyday = 0, int* pisdst = 0) const
	{
		struct tm* ptm = ::localtime(&t_); // !!!not thread safe

		ensure (ptm);

		if (py) 
			*py = ptm->tm_year + 1900;
		if (pm) 
			*pm = ptm->tm_mon + 1;
		if (pd) 
			*pd = ptm->tm_mday;
		if (ph) 
			*ph = ptm->tm_hour;
		if (pn) 
			*pn = ptm->tm_min;
		if (ps) 
			*ps = ptm->tm_sec;
		if (pwday) 
			*pwday = ptm->tm_wday;
		if (pyday) 
			*pyday = ptm->tm_yday;
		if (pisdst) 
			*pisdst = ptm->tm_isdst;
	}

	// wall clock to UTC time_t
	void maketime(int y, int m, int d, int h = 0, int n = 0, int s = 0)
	{
		struct tm t;

		t.tm_year = y - 1900;
		t.tm_mon  = m - 1;
		t.tm_mday = d;
		t.tm_hour = h;
		t.tm_min  = n;
		t.tm_sec  = s;
		t.tm_isdst = -1;

		t_ = ::mktime(&t);
		ensure(is_valid());
	}

	// get day of week, Sunday = 0
	int weekday() const
	{
		int wday;
		
		localtime(0, 0, 0, 0, 0, 0, &wday);

		return wday;
	}
	// get day of year, Jan 1 = 0
	int yearday() const
	{
		int yday;

		localtime(0, 0, 0, 0, 0, 0, 0, &yday);

		return yday;
	}

	int second() const
	{
		int sec;

		localtime(0, 0, 0, 0, 0, &sec);

		return sec;
	}
	int minute() const
	{
		int min;

		localtime(0, 0, 0, 0, &min);

		return min;
	}
	int hour() const
	{
		int hour;

		localtime(0, 0, 0, &hour);

		return hour;
	}
	int day() const
	{
		int mday;

		localtime(0, 0, &mday);

		return mday;
	}
	int month() const
	{
		int mon;

		localtime(0, &mon);

		return mon;
	}
	int year() const
	{
		int year;

		localtime(&year);

		return year;
	}

	// 1 if DST, 0 if not, -1 if no DST info
	int is_dst() const
	{
		int dst;

		localtime(0, 0, 0, 0, 0, 0, 0, 0, &dst); 

		return dst;
	}
	// True if date is a leap year.
	int is_leap() const
	{
		int y = year();

		return y%4 == 0 && (y%100 != 0 || y%400 == 0); 
	}
	// date in YYYYMMDD format
	int ymd() const
	{
		int y, m, d;

		localtime(&y, &m, &d);

		return y*10000 + m*100 + d;
	}
	// time in HHMMSS format
	int hms() const
	{
		int h, m, s;

		localtime(0, 0, 0, &h, &m, &s);

		//return h/1e2 + n/1e4 + s/1e6;
		return h*10000 + m*100 + s;
	}
	// Detect if day falls in holiday calendar(s)
	// Assumes holidays are sorted.
	bool is_holiday(const holiday_calendar& cal = CALENDAR_NONE) const
	{
		return cal(*this);
	}

	bool is_workday() const
	{ 
		int w = weekday();

		return w != DAY_SAT && w != DAY_SUN; 
	}
	// business day
	bool is_bday(const holiday_calendar& cal = CALENDAR_NONE) const
	{
		return is_workday() && !is_holiday(cal);
	}
	// wrapper for ANSI C difftime() function
	double difftime(const date& d) const
	{
		return ::difftime(t_, d.time()); 
	}

	// difference in seconds between dates
	double diffseconds(const date& d) const
	{
		return difftime(d);
	}

	int diffdays(const date& d) const
	{
		return static_cast<int>((difftime(d) + SECS_PER_DAY/2)/SECS_PER_DAY);
	}

	// difference in years between dates
	double diffyears(const date& d) const
	{
		return difftime(d)/SECS_PER_YEAR; 
	}
	date& addyears(double y)
	{
		t_ += static_cast<time_t>(y*SECS_PER_YEAR);

		return *this;
	}
	date addyears(double y) const
	{
		date d(*this);

		d.addyears(y);

		return d;
	}

	// number of business days between dates
	int diffworkdays(date d1, const holiday_calendar& cal = CALENDAR_NONE) const
	{
		int count;
		int y, m, d;
		
		localtime(&y, &m, &d);
		date d0(y, m, d);
		d1.localtime(&y, &m, &d);
		d1 = date(y, m, d);

		count = is_bday(cal);
		if (d0 == d1)
			return count; // like excel

		if (d0 < d1) {
			count = -count;
			do {
				d0.incr(1);
				if (d0.is_bday(cal))
					--count;
			} while (d0 < d1);
		}
		else {
			do {
				d0.incr(-1);
				if (d0.is_bday(cal))
					++count;
			} while (d1 < d0);
		}

		return count;
	}

	// day count fractions
	// http://www.eclipsesoftware.biz/DayCountConventions.html

	double diff_30u_360(const date& d) const
	{
		int y2, m2, d2, y1, m1, d1;

		localtime(&y2, &m2, &d2);
		d.localtime(&y1, &m1, &d1);

		if (d2 == 31 && (d1 == 30 || d1 == 31)) d2 = 30;
		if (d1 == 31) d1 = 30;

		return y2 - y1 + (m2 - m1)/12. + (d2 - d1)/360.;
	}

	double diff_30e_360(const date& d) const
	{
		int y2, m2, d2, y1, m1, d1;

		localtime(&y2, &m2, &d2);
		d.localtime(&y1, &m1, &d1);

		if (d1 == 31) d1 = 30;
		if (d2 == 31) d2 = 30;
		
		return (y2 - y1) + (m2 - m1)/12. + (d2 - d1)/360.;
	}

	double diff_actual_360(const date& d) const
	{
		return diffdays(d)/360.; 
	}

	double diff_actual_365(const date& d) const
	{
		return diffdays(d)/365.; 
	}

	// ISDA computation
	double diff_actual_actual_isda(const date& d) const
	{
		double dcf;

		if (year() != d.year()) {
			date d0(year(), 1, 1);
			
			dcf = diffdays(d0)/(365. + is_leap())
				  + d0.diffdays(d)/(365. + d.is_leap());
		}
		else {
			dcf = diffdays(d)/(365. + is_leap());
		}

		return dcf;
	}

	// ICMA computation
	double diff_actual_actual_icma(const date& d) const
	{
		double dcf;

		if (year() != d.year()) {
			date d0(year(), 1, 1);
			
			dcf = diffdays(d0)/(365. + is_leap())
				  + d0.diffdays(d)/(365. + d.is_leap());
		}
		else {
			dcf = diffdays(d)/(365. + is_leap());
		}

		return dcf;
	}

	double diff_dcb(const date& d, day_count_basis b) const
	{
		double t;

		switch(b) {
		case DCB_ACTUAL_YEARS:
			t = diffyears(d);
			break;
		case DCB_30U_360:
			t = diff_30u_360(d);
			break;
		case DCB_30E_360:
			t = diff_30e_360(d);
			break;
		case DCB_ACTUAL_360:
			t = diff_actual_360(d);
			break;
		case DCB_ACTUAL_365:
			t = diff_actual_365(d);
			break;
		case DCB_ACTUAL_ACTUAL_ISDA:
			t = diff_actual_actual_isda(d);
			break;
		case DCB_ACTUAL_ACTUAL_ICMA:
			t = diff_actual_actual_icma(d);
			break;
		default:
			t = std::numeric_limits<double>::quiet_NaN();
		}

		return t;
	}

	// adjust date based on roll convention and holiday calendar
	date& adjust(roll_convention roll, const holiday_calendar& cal = CALENDAR_NONE)
	{
		int m, one = 1;

		switch (roll) {
			case ROLL_NONE:
				// next non holiday
				while (is_holiday(cal))
					incr(1);
				break;
				// following business day
			case ROLL_FOLLOWING_BUSINESS:
				one *= -1;
				// previous business day
			case ROLL_PREVIOUS_BUSINESS:
				one *= -1;
				while (!is_bday(cal))
					incr(one);
				break;
				// earlier of following and last business day of month
			case ROLL_MODIFIED_FOLLOWING:
				one *= -1;
				// later of previous and first business day of month
			case ROLL_MODIFIED_PREVIOUS:
				one *= -1;
				m = month();
				if (!is_bday(cal)) {
					incr(one, UNIT_BUSINESS_DAY, cal);
					if (m != month())
						incr(-one, UNIT_BUSINESS_DAY, cal);
				}
				break;
			default:
				t_ = (time_t)-1;
		}

		return *this;
	}
	// move a date to n-th week day 
	date& imm(int nth, day_of_week day)
	{
		int y, m, d, h, n, s, wday;
		
		ensure (nth >= 0);

		localtime(&y, &m, &d, &h, &n, &s, &wday);
		if (nth == 0) {
			// increment to day of week
			// stay put if already there
			d += day - wday + 7*(day < wday);
		}
		else {
			// could be more efficient!!!
			maketime(y, m, 1, h, n, s);
			localtime(&y, &m, &d, &h, &n, &s, &wday);
			d = 1 + day - wday + 7*(day < wday);
			ensure (1 <= d && d <= 7);
			d = d + 7*(nth - 1);
		}
		maketime(y, m, d, h, n, s);

		return *this;
	}
	date& incr(int count, time_unit unit, const holiday_calendar& cal = CALENDAR_NONE)
	{
		int d0;
		int one = count > 0 ? 1 : -1;

		int y, m, d, h, n, s;
		localtime(&y, &m, &d, &h, &n, &s);

		switch (unit) {
			case UNIT_SECONDS:
				s += count;
				maketime(y, m, d, h, n, s);
				break;
			case UNIT_MINUTES:
				n += count;
				maketime(y, m, d, h, n, s);
				break;
			case UNIT_HOURS:
				h += count;
				maketime(y, m, d, h, n, s);
				break;
			case UNIT_DAYS:
				d += count;
				maketime(y, m, d, h, n, s);				
				break;
			case UNIT_WEEKS:
				d += 7*count;
				maketime(y, m, d, h, n, s);
				break;
			case UNIT_MONTHS:
				maketime(y, m + count, d, h, n, s);
				localtime(&y, &m, &d0, &h, &n, &s);
				if (d != d0) {
					// went into next month
					incr(0, UNIT_FIRST_OF_MONTH);
					incr(-1, UNIT_DAY);
				}
				break;
			case UNIT_YEARS:
				y += count;
				maketime(y, m, d, h, n, s);
				break;
			case UNIT_BUSINESS_DAYS:
				if (count < 0) {
					count = -count;
				}
				while (0 != count) {
					d += one;
					maketime(y, m, d, h, n, s);
					if (is_bday(cal)) {
						count--;
					}
				}
				break;
			case UNIT_FIRST_OF_MONTH:
				if (count)
					incr(count, UNIT_MONTHS);
				localtime(&y, &m, &d, &h, &n, &s);
				maketime(y, m, 1, h, n, s);
				break;
			case UNIT_END_OF_MONTH:
				incr(count + 1, UNIT_MONTHS);
				incr(0, UNIT_FIRST_OF_MONTH);
				incr(-1, UNIT_DAY);
				break;
			default:
				ensure(!"date::incr: unknown unit");
		}

		return *this;
	}
}; // class date

	struct interval {
		int count_; time_unit unit_; // e.g., 2, UNIT_WEEKS
		day_count_basis dcb_;
		roll_convention roll_;
		holiday_calendar cal_;
		interval(int count, time_unit unit, day_count_basis dcb, roll_convention roll, holiday_calendar cal)
			: count_(count), unit_(unit), dcb_(dcb), roll_(roll), cal_(cal)
		{ }
	};

	inline std::vector<date> schedule(const datetime::date& eff, int count, time_unit unit,
			payment_frequency freq, roll_convention roll, const datetime::holiday_calendar& cal)
	{
		ensure (0 < freq && freq <= FREQ_WEEKLY);

		std::vector<date> d_;
		d_.push_back(eff); // adjust ???

		datetime::date term(eff);
		term.incr(count, unit).adjust(roll, cal);

		for (size_t i = 1; true; ++i) {
			d_.push_back(datetime::date(eff).incr(i*12/freq, UNIT_MONTHS).adjust(roll, cal));
			if (d_.back().diffyears(term) >= 0)
				break;
		}
	
		return d_;
	}


// encode from or decode to
template <class T>
struct date_convert {
	static T     encode(date d);
	static date  decode(T t);
};

// traits for the date class
template<> inline double
date_convert<double>::encode(date d)
{
	return d.excel(); 
}
template<> inline date
date_convert<double>::decode(double d)
{
	return date(d); 
}

template<> inline time_t 
date_convert<time_t>::encode(date d)
{
	return d.time(); 
}
template<> inline date
date_convert<time_t>::decode(time_t d)
{
	return date(d); 
}

template<> inline date
date_convert<date>::encode(date d)
{
	return d;
}
template<> inline date
date_convert<date>::decode(date d)
{
	return d; 
}

template<> inline int
date_convert<int>::encode(date d)
{
	return d.ymd(); 
}
template<> inline date
date_convert<int>::decode(int ymd)
{ 
	return date(ymd/10000, (ymd - ymd/10000)/100, ymd%100); 
}
/*
// generate an interval of dates with possible odd date
template <class T>
void
datetime_periodic(T begin, T end, payment_frequency freq, roll_convention roll,
	size_t ncal, calendar_pointer cal, int& n, T* t)
{
	int count, one = 1;
	time_unit unit;

	if (freq < 0) {
		one = -1;
		freq *= one;
	}

	date b = date_convert<T>::decode(begin);
	ensure (b.is_valid());

	date e = date_convert<T>::decode(end);
	ensure (e.is_valid());

	if (one == -1) {
		std::swap(b, e);
	}

	if (freq == FREQ_DAILY) {
		count = 1;
		unit = UNIT_DAY;
	}
	else if (freq == FREQ_WEEKLY) {
		count = 1;
		unit = UNIT_WEEK;
	}
	else if (freq == FREQ_BIWEEKLY) {
		count = 2;
		unit = UNIT_WEEKS;
	}
	else {
		count = 12/freq;
		unit = UNIT_MONTHS;
	}

	for (int i = 0; i < n; ++i) {
		date d;

		d = date(b).incr(i*count*one, unit);
		ensure (d.is_valid());

		if (one*e.diffdays(d) <= 0) {
			n = i;	// break out of loop
		}

		d.adjust(roll, ncal, cal);
		t[i] = date_convert<T>::encode(d);
	}
	e.adjust(roll, ncal, cal);
	t[n] = date_convert<T>::encode(e);
	n++; // real value of n

	if (one == -1)
		std::reverse(t, t + n);

	return 0;
}
*/
} // namespace datetime
/*
inline bool operator==(const datetime::date& t0, const datetime::date& t1)
{
	return t0.t_ == t1.t_;
}
inline double operator<(const datetime::date& t0, const datetime::date& t1)
{
	return t0.t_ < t1.t_;
}inline double operator-(const datetime::date& t0, const datetime::date& t1)
{
	return t0.diffyears(t1);
}
inline datetime::date operator+(const datetime::excel_date t0, double dt)
{
	datetime::date t1(t0);

	t1.addyears(dt);

	return t1;
}
*/