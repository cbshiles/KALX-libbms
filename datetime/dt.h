// dt.h - Lightweight date and time routines.
// Copyright (c) 2011 KALX, LLC. All rights reserved. No warranty is made.
#pragma once
#pragma warning(disable: 4996) // _timezone warings. Should use _get_timezone.
#ifndef ensure
#include <cassert>
#define ensure(x) assert(x)
#endif
#include <cmath>
#include <ctime>
#include <utility>

using namespace std::rel_ops;

namespace datetime {

	static const double SECS_PER_DAY
		= 24.*60*60;
	static const double DAYS_PER_YEAR
		= 365.25;
	static const double SECS_PER_YEAR
		= SECS_PER_DAY*DAYS_PER_YEAR;

	// Excel Julian date for 1 Jan 1970
	static const double EXCEL_19700101
		= 25569;
	static const double EXCEL_EPOCH
		= EXCEL_19700101;

	// Excel Julian date for 1/19/2038 3:14.
	static const double EXCEL_ERA
		= EXCEL_EPOCH + 0xFFFFFFFF/SECS_PER_DAY;

	// Excel to time in years since epoch.
	inline double
	excel2years(double t, double epoch)
	{
		return (t - epoch)/DAYS_PER_YEAR;
	}
	inline double
	years2excel(double y, double epoch)
	{
		return y * DAYS_PER_YEAR + epoch;
	}

	// daylight savings time adjustment
	inline long 
	dst(time_t t) 
	{
		struct tm* ptm;
		
		ensure (0 != (ptm = ::localtime(&t))); // too slow!!!

		return ptm->tm_isdst*3600;
	}

	// Excel local time to UTC.
	// Should agree with mktime for broken down time.
	inline time_t
	excel2time_t(double d, bool nodst = false)
	{
		ensure (EXCEL_EPOCH <= d); //!!! && d <= EXCEL_ERA)

		time_t t = static_cast<time_t>(0.5 + _timezone + (d - EXCEL_EPOCH)*SECS_PER_DAY);

		return t - (nodst ? 0 : dst(t));
	}
	// UTC to Excel local time.
	inline double
	time_t2excel(time_t t, bool nodst = false)
	{
		return EXCEL_EPOCH + (t - _timezone + (nodst ? 0 : dst(t)))/SECS_PER_DAY; 
	}

	// breakdown double of the form yyyymmdd.hhnnss
	inline void
	breakdown(double d, int* py = 0, int* pm = 0, int* pd = 0, int* ph = 0, int* pn = 0, int* ps = 0)
	{
		int t = static_cast<int>(0.5 + 1e6*fmod(d, 1));

		if (py) {
			*py = static_cast<int>(d/1e4);
			if (pm) {
				*pm = static_cast<int>((d - 1e4* *py)/1e2);
				if (pd) {
					*pd = static_cast<int>(d - 1e4* *py - 1e2* *pm);
				}
			}
		}
		if (ph) {
			*ph = static_cast<int>(t/1e4);
			if (pn) {
				*pn = static_cast<int>((t - 1e4* *ph)/1e2);
				if (ps) {
					*ps = static_cast<int>(t - 1e4* *ph - 1e2* *pn);
				}
			}
		}
	}

} // namespace datetime
