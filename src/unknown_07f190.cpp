// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_07F190.CPP: the trimmed mean of a set of samples (lane D: called
   by the session's member latency summary and the observer) */

#include "unknown_11c920.h"
#include <string.h>

/* the fraction of the samples dropped at each end */
real *g_4cf8e8;

/* sorts the samples, drops the given fraction of them at each end and returns
   the mean of the rest */
// @retail 0x7f190
long samples_trimmed_mean(const long *samples, long count)
{
	long sorted[128];
	long trim = (long)(count * *g_4cf8e8);
	long used;
	long total;
	long i;

	if (trim > 0)
	{
		for (i = 0; i < count; i++)
		{
			for (long j = i; j >= 0; j--)
			{
				if (j <= 0 || sorted[j - 1] <= samples[i])
				{
					sorted[j] = samples[i];
					break;
				}
				sorted[j] = sorted[j - 1];
			}
		}
		used = i - 2 * trim;
		memcpy(sorted, &sorted[trim], used * sizeof(long));
	}
	else
	{
		for (i = 0; i < count; i++)
			sorted[i] = samples[i];
		used = count;
	}
	total = 0;
	for (i = 0; i < used; i++)
		total += sorted[i];
	return total / used;
}
