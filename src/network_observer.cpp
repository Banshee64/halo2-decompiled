// @flags /O2 /Ob1 /arch:SSE /Gr
/* NETWORK_OBSERVER.CPP: the network observer's owners and channels (lane D,
   outside its regions: the session code sends through it) */

#include "cseries.h"
#include "globals.h"
#include "network_observer.h"
#include "network_session.h"
#include <xtl.h>
#include <string.h>

struct s_session_machine_address
{
	byte data[6];
};

/* the security keys (0x7a9a0) and the transport */
bool function_07ab10(long key_index, transport_address *address, long local, word port, const XNADDR *xnaddr);
bool function_07acc0(const transport_address *address);

// @retail 0x75870
long network_time_get(void)
{
	if (g_510548)
		return g_51054c;
	return GetTickCount();
}

// @retail 0x75890
long network_time_since(long time)
{
	long now;
	if (g_510548)
		now = g_51054c;
	else
		now = GetTickCount();
	return now - time;
}

/* the sessions (g_510550; network_message_handler.cpp) */
struct s_network_session_list;
s_network_session_list *g_510550;
c_network_session *network_session_manager_find_session(s_network_session_list *manager, const s_session_id *session_id);

/* how long ago the session with this id started, or 0 */
// @retail 0x758c0
long network_session_time_since_start(const s_session_id *session_id)
{
	long result = 0;
	s_network_session_list *manager = g_510550;
	if (manager)
	{
		c_network_session *session = network_session_manager_find_session(manager, session_id);
		if (session && session->flag78ac)
			result = GetTickCount() + session->time78b0;
	}
	return result;
}

/* the object a connection reports to */
struct s_network_connection_owner
{
	byte unknown00[0x30];
	bool active;
};

// @retail 0x75910
long network_connection_owner_active(s_network_connection *connection)
{
	s_network_connection_owner *owner = (s_network_connection_owner *)connection->callback;
	if (owner && owner->active)
		return 1;
	return 0;
}

// @retail 0x75930
bool network_connection_get_address(s_network_connection *connection, transport_address *address)
{
	bool result = false;
	if (connection->state != 0 && connection->state != 1)
	{
		*address = connection->address;
		result = true;
	}
	return result;
}

// @retail 0x75a90
void network_observer_set_owner_key(s_network_observer *observer, long owner, const s_network_session_id *id, const byte *key, long key_index, long local)
{
	if (key_index != NONE)
	{
		observer->owners[owner].key_index = key_index;
		s_network_observer_owner *record = &observer->owners[owner];
		record->local = local;
		record->id = *id;
		*(XNKEY *)record->key = *(const XNKEY *)key;
	}
	else
	{
		observer->owners[owner].key_index = NONE;
	}
}

// @retail 0x75ce0
long network_observer_find_channel(s_network_observer *observer, long owner, long remote_index)
{
	long result = NONE;

	for (long i = 0; i < MAXIMUM_OBSERVER_CHANNELS; i++)
	{
		s_network_observer_channel *channel = &observer->channels[i];
		if (channel->state && channel->connection_index == remote_index && (channel->owner_mask & (1 << owner)))
		{
			result = i;
			break;
		}
	}
	return result;
}

// @retail 0x75d30
long network_observer_find_channel_by_machine(s_network_observer *observer, const s_session_machine_address *address, long owner)
{
	for (long i = 0; i < MAXIMUM_OBSERVER_CHANNELS; i++)
	{
		s_network_observer_channel *channel = &observer->channels[i];
		if (channel->state && (channel->owner_mask & (1 << owner)))
		{
			s_session_machine_address machine = *(s_session_machine_address *)((byte *)channel->remote_id + 0xa);
			if (memcmp(address, &machine, sizeof(machine)) == 0)
				return i;
		}
	}
	return NONE;
}

// @retail 0x75e40
void network_observer_close_channel(s_network_observer *observer, long channel_index)
{
	s_network_observer_channel *channel = &observer->channels[channel_index];
	if (channel->connection_index != NONE)
	{
		s_network_connection *connection = network_connection_get(channel->connection_index);
		if (connection->state > 2)
			network_connection_close(connection, 0x11);
	}
}

/* the transport address of an owner's machine, from its secure key */
// @retail 0x783d0
bool network_observer_get_owner_address(s_network_observer *observer, long owner, const XNADDR *xnaddr, transport_address *address, long *key_index, s_network_session_id *id, XNKEY *key)
{
	/* retail keeps the result in a stack slot (a volatile local reproduces it) */
	volatile bool result = false;

	if (observer->owners[owner].active && observer->owners[owner].key_index != NONE)
	{
		transport_address secure_address;
		if (function_07ab10(observer->owners[owner].key_index, &secure_address, observer->owners[owner].local, 1000, xnaddr) && function_07acc0(&secure_address))
		{
			*address = secure_address;
			*key_index = observer->owners[owner].key_index;
			*id = observer->owners[owner].id;
			*key = *(XNKEY *)observer->owners[owner].key;
			return true;
		}
	}
	memset(address, 0, sizeof(*address));
	return result;
}

// @retail 0x75e80
void network_observer_send_message(s_network_observer *observer, long owner, long channel_index, bool out_of_band, long message_type, long message_size, const void *message)
{
	s_network_observer *const *observer_reference = &observer;
	s_network_observer_channel *channel = &(*observer_reference)->channels[channel_index];

	if (channel->connection_index != NONE)
	{
		s_network_connection *connection = network_connection_get(channel->connection_index);
		if (out_of_band)
		{
			transport_address address;
			if (transport_address_valid(&channel->address))
			{
				address = channel->address;
			}
			else
			{
				long key_index;
				s_network_session_id id;
				XNKEY key;
				if (!network_observer_get_owner_address(observer, owner, (const XNADDR *)channel->remote_id, &address, &key_index, &id, &key))
					return;
			}
			function_07b140(observer->link, (long)&address, message_type, message_size, (void *)message);
		}
		else if (connection->state > 2)
		{
			unsigned __int64 bit = (unsigned __int64)1 << message_type;
			if (channel->message_mask & bit)
				channel->message_mask &= ~bit;
			function_095580(network_stream_get(connection->stream_index), message_type, message_size, message);
		}
	}
}

// @retail 0x768b0
bool network_observer_channel_ready(s_network_observer *observer, long channel_index, long message_type)
{
	s_network_observer_channel *channel = &observer->channels[channel_index];
	bool result = false;

	if (channel->connection_index != NONE)
	{
		s_network_connection *connection = network_connection_get(channel->connection_index);
		if (connection->state == 5)
		{
			if (!(channel->message_mask & ((unsigned __int64)1 << message_type)))
				result = network_connection_send_capacity(connection) * 4 < 0x4000;
			else
				result = network_connection_send_capacity(connection) * 4 < 0xc000;
		}
	}
	return result;
}

// @retail 0x76930
void network_observer_mark_message(s_network_observer *observer, long channel_index, long message_type)
{
	s_network_observer_channel *channel = &observer->channels[channel_index];

	if (channel->connection_index != NONE && network_connection_get(channel->connection_index)->state == 5)
	{
		unsigned __int64 bit = (unsigned __int64)1 << message_type;
		if (!(channel->message_mask & bit))
			channel->message_mask |= bit;
	}
}

/* network_time_get, which retail inlines here */
static inline long observer_time_get(void)
{
	if (g_510548)
		return g_51054c;
	return GetTickCount();
}

// @retail 0x77330
void network_observer_set_channel_state(s_network_observer *observer, long state, long channel_index)
{
	s_network_observer_channel *channel = &observer->channels[channel_index];
	if (channel->state != state)
	{
		channel->state = state;
		channel->time = observer_time_get();
		if (channel->state == 1)
		{
			for (long i = 0; i < MAXIMUM_OBSERVER_OWNERS; i++)
			{
				if (channel->owner_mask & (1 << i))
					observer->owners[i].active->channel_closed(channel_index);
			}
		}
	}
}

// @retail 0x75c80
bool network_observer_get_bandwidth(s_network_observer *observer, long *value4e08, real *ratio, long *value4e0c)
{
	bool result = false;
	if (observer->value4e08 != NONE && observer->value4e0c != NONE)
	{
		long count = observer->value4e18;
		if (observer->value4e1c + count > 0)
		{
			real fraction = (real)observer->value4e1c / (real)(observer->value4e1c + count);
			*value4e08 = observer->value4e08;
			*ratio = fraction;
			*value4e0c = observer->value4e0c;
			result = true;
		}
	}
	return result;
}

// @retail 0x769a0
bool network_observer_channel_timed_out(s_network_observer *observer, long channel_index)
{
	s_network_observer *const *observer_reference = &observer;
	s_network_observer_channel *channel = &(*observer_reference)->channels[channel_index];
	bool result = false;
	if (channel->connection_index != NONE)
	{
		s_network_connection *connection = network_connection_get(channel->connection_index);
		if (connection->state == 5)
		{
			long last = connection->timers[1].time;
			long since = observer_time_get() - last;
			long time = connection->state > 2 ? connection->timers[4].time : 0;
			long now = observer_time_get();
			if (since < observer->configuration->timeout78 && now - time >= observer->configuration->timeout7c)
				result = false;
			else
				result = true;
		}
	}
	return result;
}

/* closes a channel's connection when nothing has come over it for too long */
// @retail 0x773a0
void network_observer_check_channel_activity(s_network_observer *observer, long channel_index)
{
	s_network_observer *const *observer_reference = &observer;
	s_network_observer_channel *channel = &(*observer_reference)->channels[channel_index];
	if (channel->state)
	{
		if (channel->connection_index != NONE)
		{
			s_network_connection *connection = network_connection_get(channel->connection_index);
			if (connection->state == 5)
			{
				long last = connection->timers[0].time;
				if (observer_time_get() - last < observer->configuration->timeout74)
				{
					long since_activity = network_time_since(channel->time94);
					long since_timer = network_time_since(connection->state > 2 ? connection->timers[3].time : 0);
					if (since_activity >= observer->configuration->timeout80 && since_timer >= observer->configuration->timeout84)
						network_connection_close(connection, 0x10);
					return;
				}
			}
		}
		channel->time94 = observer_time_get();
	}
}
/* the security code's connect status of an address (unknown_07a9a0.cpp) */
long function_07acf0(const transport_address *address);

/* the connect status of a channel's address (0 when it has none) */
// @retail 0x78580
long network_observer_channel_connect_status(s_network_observer *observer, long channel_index)
{
	long result = 0;
	s_network_observer_channel *channel = &observer->channels[channel_index];
	transport_address *address = &channel->address;
	if (transport_address_valid(address))
		result = function_07acf0(address);
	return result;
}

// @retail 0x78150
long network_observer_scaled_size(s_network_observer *observer, bool flag, real scale)
{
	long count = flag ? observer->configuration->value10c : observer->configuration->value108;
	long result;

	/* rounds as the x87 does (real_math's fld/fistp idiom) */
	scale = (real)(count * 8 + 0x168) * scale;
	__asm
	{
		fld scale
		fistp result
	}
	return result;
}
