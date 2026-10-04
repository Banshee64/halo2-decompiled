/* NETWORK_OBSERVER.H: the network observer (lane D): up to four owners (the
   sessions), each with its secure key, and fifteen channels (0x528 bytes each)
   that carry their messages over a connection (0x4d87d4, 0xf8 bytes each) */

#ifndef NETWORK_OBSERVER_H
#define NETWORK_OBSERVER_H

#include "cseries.h"
#include "transport_address.h"
#include "network_connection.h"
#include "network_channel_owner.h"
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
	c_network_channel_owner *active;
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
	short attempts;
	long connection_index;
	long unknown10;
	dword remote_id[9];
	dword owner_flags;
	long owner_index;
	long key_index;
	s_network_session_id id;
	XNKEY key;
	transport_address address;
	long qos_handle;
	byte unknown74[0x94 - 0x74];
	long time94;
	byte unknown98[0x520 - 0x98];
	unsigned __int64 message_mask;
};

/* the observer's timeouts and limits (0x4cf4e0) */
struct s_network_observer_configuration
{
	byte unknown00[0x74];
	long timeout74;
	long timeout78;
	long timeout7c;
	long timeout80;
	long timeout84;
	long timeout88;
	long timeout8c;
	byte unknown90[0x108 - 0x90];
	long value108;
	long value10c;
	real real110;
	byte unknown114[0x134 - 0x114];
	long value134;
	long value138;
};

/* what the connections report their traffic to (the observer, vtable
   0x450e10) */
class c_network_connection_listener
{
public:
	virtual void packet_sent(long connection_index, long size, bool flag) {}
	virtual void connection_updated(long connection_index, long value) {}
	virtual void packet_received(long connection_index, long a, long size) {}
	virtual void connection_v03(long connection_index, bool a, bool b) {}
	virtual void connection_v04(long connection_index) {}
	virtual bool connection_v05(long connection_index, bool a, bool b, bool c, long d, long e, long f, long g, long h, long i) { return false; }
};

struct s_network_observer : public c_network_connection_listener
{
	void connection_updated(long connection_index, long value);
	void *unknown04;
	void *link;
	void *unknown0c;
	s_network_observer_configuration *configuration;
	s_network_observer_owner owners[MAXIMUM_OBSERVER_OWNERS];
	byte unknowna4[4];
	s_network_observer_channel channels[MAXIMUM_OBSERVER_CHANNELS];
	byte flag4e00;
	byte unknown4e01[3];
	long value4e04;
	long value4e08;
	long value4e0c;
	long value4e10;
	byte flag4e14;
	byte unknown4e15[3];
	long value4e18;
	long value4e1c;
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
