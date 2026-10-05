/* UNKNOWN_092870_2.H: the traffic statistics of a link direction, a
   connection and an observer channel (unknown_092870.cpp, unknown_075870.cpp) */

#ifndef NETWORK_STATISTICS_H
#define NETWORK_STATISTICS_H

#include "unknown_11c920.h"

#define NUMBER_OF_STATISTICS_SAMPLES 20

struct s_network_traffic
{
	long packets;
	long bytes;
};

static inline void network_traffic_clear(s_network_traffic *traffic)
{
	traffic->packets = 0;
	traffic->bytes = 0;
}

/* the traffic of one direction of a link, in total and over the last
   NUMBER_OF_STATISTICS_SAMPLES periods */
struct s_network_statistics
{
	unsigned __int64 packets;
	unsigned __int64 bytes;
	dword period_start;
	s_network_traffic current;
	long interval;
	long period;
	real rate_scale;
	long sample_index;
	s_network_traffic samples[NUMBER_OF_STATISTICS_SAMPLES];
	s_network_traffic total;
	long unknownd4;
};

void network_statistics_initialize(s_network_statistics *statistics, long interval);
void network_statistics_update(s_network_statistics *statistics);

/* the traffic one packet adds to a direction's statistics */
static inline void network_statistics_add(s_network_statistics *statistics, long size)
{
	network_statistics_update(statistics);
	statistics->packets++;
	statistics->bytes += size;
	statistics->current.packets++;
	statistics->current.bytes += size;
}

#endif