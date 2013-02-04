// datetime.cpp - date and time functions
// Copyright (c) 2011 KALX, LLC. All rights reserved. No warranty is made.
#include "datetime.h"

using namespace System;
//using namespace System::ComponentModel;
using namespace System::Reflection;

namespace DateTime {


	/// <summary>Date and time functions.</summary>
	public ref class Date {
		datetime::date* p_;
	public:
		/// <summary>Construct date from year, month, and day</summary>
		/// <param name="y">is the year</param>
		/// <param name="m">is the month of the year</param>
		/// <param name="d">is the day of the month</param>
		/// <remarks></remarks>
		Date(int y, int m, int d)
			: p_(new datetime::date(y,m,d))
		{ }
		~Date()
		{
			delete p_;
		}
	/// <summary>Units of time</summary>
	enum class Time_Unit {
		[Description("seconds")]
		UNIT_SECOND = 1, UNIT_SECONDS = 1, /// <summary>summary</summary>
		UNIT_MINUTE = 2, UNIT_MINUTES = 2,
		UNIT_HOUR   = 3, UNIT_HOURS   = 3,
		UNIT_DAY    = 4, UNIT_DAYS    = 4,
		UNIT_WEEK   = 5, UNIT_WEEKS   = 5,
		UNIT_MONTH  = 6, UNIT_MONTHS  = 6,
		UNIT_YEAR   = 7, UNIT_YEARS   = 7,
		UNIT_BUSINESS_DAY  = 9,
		UNIT_BUSINESS_DAYS = 9,
		UNIT_END_OF_MONTH = 10,
		UNIT_FIRST_OF_MONTH, /// First day of the month???
	};

	};

} // namespace DateTime