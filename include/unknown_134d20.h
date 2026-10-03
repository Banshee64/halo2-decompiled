/* UNKNOWN_134D20.H: the scenario's named interpolators (src/unknown_134d20.cpp) */

#ifndef UNKNOWN_134D20_H
#define UNKNOWN_134D20_H

#include "cseries.h"

/* the state of an interpolator, 0x20 bytes */
struct s_interpolator_state
{
	real value;
	real start_value;
	real target_value;
	real start_time;
	real time10;
	real end_time;
	real value18;
	byte active : 1;
	byte flag1 : 1;
	byte : 6;
	byte unknown1d[3];
};

s_interpolator_state *interpolator_get(long name, long *index_out);
long interpolator_start(long name, real target, real seconds);
long interpolator_resume(long name);
bool interpolator_exists(long name);
long function_135180(long name, real target, real seconds);
real interpolator_get_value18(long name);
real interpolator_get_time10(long name);
real interpolator_get_end_time(long name);

#endif
