// @flags /O2 /Gr
/* UNKNOWN_062F40.CPP: network session reservations */

#include "unknown_11c920.h"
#include <string.h>

#define MAXIMUM_RESERVATIONS 16

struct s_reservation
{
	byte active;
	byte unknown01[9];
	byte identity[12];
	byte unknown16[0x24 - 0x16];
};

struct s_reservation_session
{
	byte unknown00[0x7668];
	s_reservation reservations[MAXIMUM_RESERVATIONS];
};

// @retail 0x62f40
bool function_062f40(s_reservation_session *session, const void *identity, s_reservation **reservation_out)
{
	s_reservation *reservation = session->reservations;
	s_reservation *end = reservation + MAXIMUM_RESERVATIONS;
	bool result = false;
	for (; reservation < end; reservation++)
	{
		if (reservation->active && !memcmp(identity, reservation->identity, 12))
		{
			result = true;
			if (reservation_out)
			{
				*reservation_out = reservation;
			}
			break;
		}
	}
	return result;
}
