/* NETWORK_OBSERVER.H: the network observer (lane D): up to four owners (the
   sessions), each with its secure key, and fifteen channels (0x528 bytes each)
   that carry their messages over a connection (0x4d87d4, 0xf8 bytes each) */

#ifndef NETWORK_OBSERVER_H
#define NETWORK_OBSERVER_H

#include "cseries.h"
#include "transport_address.h"
#include "network_connection.h"
#include <xtl.h>

#define MAXIMUM_OBSERVER_OWNERS 4
#define MAXIMUM_OBSERVER_CHANNELS 15

struct s_network_session_id
{
	long a;
	long b;
};

/* the secure key an owner registered with the transport (0x24 bytes) */
struct s_network_observer_owner
{
	long active;
	long key_index;
	long local;
	s_network_session_id id;
	byte key[16];
};

/* one channel to a remote machine (0x528 bytes) */
struct s_network_observer_channel
{
	long state;
	long time;
	byte flags;
	byte owner_mask;
	short unknown0a;
	long connection_index;
	long unknown10;
	dword remote_id[9];
	byte unknown38[0x5c - 0x38];
	transport_address address;
	long unknown70;
	byte unknown74[0x520 - 0x74];
	unsigned __int64 message_mask;
};

struct s_network_observer
{
	byte unknown00[8];
	void *link;
	byte unknown0c[8];
	s_network_observer_owner owners[MAXIMUM_OBSERVER_OWNERS];
	byte unknowna4[4];
	s_network_observer_channel channels[MAXIMUM_OBSERVER_CHANNELS];
};

long network_time_get(void);
long network_time_since(long time);

void network_observer_set_owner_key(s_network_observer *observer, long owner, const s_network_session_id *id, const byte *key, long key_index, long local);
long network_observer_find_channel(s_network_observer *observer, long owner, long remote_index);
void network_observer_close_channel(s_network_observer *observer, long channel_index);
void network_observer_send_message(s_network_observer *observer, long owner, long channel_index, bool out_of_band, long message_type, long message_size, const void *message);
bool network_observer_channel_ready(s_network_observer *observer, long channel_index, long message_type);
void network_observer_mark_message(s_network_observer *observer, long channel_index, long message_type);
bool network_observer_channel_timed_out(s_network_observer *observer, long channel_index);

#endif
