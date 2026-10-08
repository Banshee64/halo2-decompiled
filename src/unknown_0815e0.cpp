// @flags /O2 /Gr
#include "unknown_11c920.h"

#include "unknown_092870.h"
#include "unknown_092870_2.h"
#include "unknown_075870.h"
#include "unknown_059ad0.h"
#include "unknown_067e10.h"
#include "network_message_types.h"
#include <string.h>
#include <stddef.h>

struct s_link_route
{
	long connection_index;
	long kind;
	bool pending;
	byte unknown09[3];
	s_type_99af70 address;
};

struct s_link_packet;
struct s_transport_endpoint;
class c_type_96c0b1;

class c_class_93590
{
public:
	c_class_93590();
	void function_93590(s_link_packet const *packet, long *size, byte *buffer, long buffer_size) const;
	bool function_93610(long size, byte const *buffer, s_link_packet *packet) const;
	bool m_initialized;
	long m_sequence;
	bool m_open;
	s_transport_endpoint *m_endpoints[4];
	c_type_96c0b1 *m_out_of_band_consumer;
	long m_route_count;
	s_link_route m_routes[16];
	long m_unknown224;
	s_network_statistics m_statistics[4];
};

void network_link_close_connections(c_class_93590 *link);
void network_link_close(c_class_93590 *link);

class c_lifecycle_link : public c_class_93590
{
public:
	~c_lifecycle_link()
	{
		network_link_close_connections(this);
		network_link_close(this);
		m_out_of_band_consumer = 0;
		m_initialized = false;
	}
};

class c_class_938e0;
struct s_network_message_gateway
{
	virtual bool read_packet(const s_type_99af70 *address, s_bitstream *stream);
	byte unknown04[4];
	c_class_93590 *link;
	c_type_659ceb *field_c_7;
	c_class_938e0 *handler;
	bool outgoing_packet_pending;
	byte outgoing_packet_storage[0x603];
	s_type_99af70 outgoing_packet_address;
	s_bitstream outgoing_packet;
};

class c_lifecycle_gateway : public s_network_message_gateway
{
public:
	c_lifecycle_gateway()
	{
		outgoing_packet.unknown08 = 1;
		outgoing_packet.size_in_bytes = 0x600;
		outgoing_packet.mode = 0;
		outgoing_packet.bit_position = 0;
		outgoing_packet.checkpoint_count = 0;
		outgoing_packet.error = false;
		outgoing_packet.data = outgoing_packet_storage;
		link = 0;
		field_c_7 = 0;
		handler = 0;
		unknown04[0] = false;
	}
	~c_lifecycle_gateway()
	{
		if (unknown04[0])
		{
			if (link)
			{
				link->m_out_of_band_consumer = 0;
				link = 0;
			}
			handler = 0;
			field_c_7 = 0;
			unknown04[0] = false;
		}
	}
};

class c_lifecycle_session : public c_class_58d20
{
public:
	c_lifecycle_session()
	{
		value14 = 0;
		unknown04 = 0;
		value18 = NONE;
		state = NONE;
		memset(&unknown1c, 0, 8);
		flag24 = false;
	}
	~c_lifecycle_session() {}
};

class c_lifecycle_watcher
{
public:
	virtual bool slot0(long) { return false; }
	virtual bool slot1(long) { return false; }
	virtual bool slot2(long, long) { return false; }
	virtual void slot3(long, long, long) {}
	virtual void slot4(long) {}
	virtual long slot5(long) { return NONE; }

	c_lifecycle_watcher()
	{
		world = 0;
		session = 0;
		link = 0;
	}
	~c_lifecycle_watcher() {}
	c_class_6a600 *world;
	void *link;
	c_class_58d20 *session;
	byte unknown10[0xc34 - 0x10];
};

struct s_lifecycle_registry
{
	s_lifecycle_registry()
	{
		entity_count = NONE;
		memset(entities, 0, sizeof(entities));
		event_count = NONE;
		memset(events, 0, sizeof(events));
	}
	long entity_count;
	void *entities[32];
	long event_count;
	void *events[32];
};

class c_lifecycle_network
{
public:
	c_lifecycle_network();
	~c_lifecycle_network();
	c_lifecycle_link link;
	c_type_659ceb collection;
	c_lifecycle_gateway gateway;
	bool handler_initialized;
	byte unknown1189[0x11a0 - 0x1189];
	s_network_observer observer;
	byte unknown60e0[0x18];
	c_lifecycle_session sessions[3];
	c_class_6a600 world;
	c_lifecycle_watcher watcher;
	s_lifecycle_registry registry;
};

typedef char lifecycle_link_size[(sizeof(c_lifecycle_link) == 0x588) ? 1 : -1];
typedef char lifecycle_gateway_size[(sizeof(c_lifecycle_gateway) == 0x660) ? 1 : -1];
typedef char lifecycle_observer_offset[(offsetof(c_lifecycle_network, observer) == 0x11a0) ? 1 : -1];
typedef char lifecycle_sessions_offset[(offsetof(c_lifecycle_network, sessions) == 0x60f8) ? 1 : -1];
typedef char lifecycle_world_offset[(offsetof(c_lifecycle_network, world) == 0x1cb20) ? 1 : -1];
typedef char lifecycle_watcher_offset[(offsetof(c_lifecycle_network, watcher) == 0x1dd40) ? 1 : -1];
typedef char lifecycle_registry_offset[(offsetof(c_lifecycle_network, registry) == 0x1e974) ? 1 : -1];

__forceinline s_network_observer::~s_network_observer()
{
}

// @retail 0x816f0
c_lifecycle_network::~c_lifecycle_network()
{
}

// @retail 0x815e0
c_lifecycle_network::c_lifecycle_network() : handler_initialized(false)
{
}
