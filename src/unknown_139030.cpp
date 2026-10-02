// @flags /O2 /Gr
/* UNKNOWN_139030.CPP */

#include "cseries.h"
#include <xtl.h>
#include <string.h>

struct s_local_time
{
	long year;
	long month;
	long day;
	long hour;
	long minute;
	long second;
};

// @retail 0x139030
void function_139030(s_local_time *time)
{
	SYSTEMTIME st;

	memset(time, 0, sizeof(s_local_time));
	GetLocalTime(&st);
	time->year = st.wYear;
	time->month = st.wMonth;
	time->day = st.wDay;
	time->hour = st.wHour;
	time->minute = st.wMinute;
	time->second = st.wSecond;
}