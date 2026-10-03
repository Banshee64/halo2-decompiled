// @flags /O2 /Gr
/* UNKNOWN_139090.CPP: the holiday of the local date */

#include "cseries.h"

struct s_local_time
{
	long year;
	long month;
	long day;
	long hour;
	long minute;
	long second;
};

void function_139030(s_local_time *time);

enum
{
	_holiday_none = 0,
	_holiday_christmas,
	_holiday_new_years_day,
	_holiday_halloween,
	_holiday_july_7,
	_holiday_independence_day,
	_holiday_december_12
};

// @retail 0x139090
long function_139090()
{
	long holiday = _holiday_none;
	s_local_time time;

	function_139030(&time);
	if (time.month == 12 && time.day == 25)
	{
		holiday = _holiday_christmas;
	}
	else if (time.month == 1 && time.day == 1)
	{
		holiday = _holiday_new_years_day;
	}
	else if (time.month == 10 && time.day == 31)
	{
		holiday = _holiday_halloween;
	}
	else if (time.month == 7 && time.day == 7)
	{
		holiday = _holiday_july_7;
	}
	else if (time.month == 7 && time.day == 4)
	{
		holiday = _holiday_independence_day;
	}
	else if (time.month == 12 && time.day == 12)
	{
		holiday = _holiday_december_12;
	}
	return holiday;
}