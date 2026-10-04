/* NETWORK_CONNECTION.H: the connections (0xf8 bytes each, at 0x4d87d4) and
   the two kinds of stream they own (lane D, src/network_connection.cpp) */

#ifndef NETWORK_CONNECTION_H
#define NETWORK_CONNECTION_H

#include "cseries.h"
#include "transport_address.h"

/* a link's list of connections (src/unknown_092e00.cpp) */
struct s_link_entry
{
	long id;
	byte unknown04[0x1c];
};

struct s_link
{
	byte unknown00[4];
	long sequence;
	byte unknown08[0x18];
	long entry_count;
	s_link_entry entries[1];
};

/* the objects that read a connection's packets and hear about its messages
   (src/network_connection.cpp) */
class c_connection_client;
class c_connection_owner;

struct s_connection_handler
{
	dword type;
	c_connection_client *client;
};

/* the connection's class: its close callback and the clients every
   connection of the class has */
struct s_connection_callback
{
	byte unknown00[4];
	void *context;
	void (__stdcall *function)(void *context);
	long handler_count;
	s_connection_handler handlers[4];
	bool active;
	bool unknown31;
};

struct s_connection_config
{
	long retry_interval;
	long retry_count;
	long connect_timeout;
	long establish_timeout;
};

struct s_connection_counter
{
	long low;
	long high;
};

struct s_connection_timer
{
	long time;
	long unknown04;
	s_connection_counter counter;
};

struct s_network_connection
{
	s_link *link_list;
	void *link;
	class c_network_message_handler *handler;
	s_connection_config const *config;
	long reliable_stream_index;
	long stream_index;
	long unknown18;
	bool unknown1c;
	bool unknown1d;
	byte unknown1e[2];
	long handler_count;
	s_connection_handler handlers[3];
	s_connection_callback *callback;
	c_connection_owner *owner;
	long id;
	dword flags;
	long local_sequence;
	long remote_sequence;
	long state;
	long close_reason;
	transport_address previous_address;
	transport_address address;
	bool initiator;
	byte unknown85[3];
	long handshake_time;
	long handshake_next_time;
	long handshake_count;
	long establish_time;
	s_connection_timer timers[6];
};

struct s_network_stream_window
{
	long unknown00;
	long unknown04;
	long next;
	long end;
};

/* 0x4d87d8: the reliable streams (0x97c bytes), 0x4d87dc: the streams (0x2850 bytes) */
struct s_network_stream_header
{
	byte unknown00[4];
	bool active;
	byte unknown05[3];
	long owner;
	void *unknown0c;
	s_network_stream_window window;
};

/* src/unknown_081f80.cpp */
extern void *g_4d87d4;
extern void *g_4d87d8;
extern void *g_4d87dc;
extern long g_4d87d0;

bool link_remove_entry(s_link *link, long id);

/* not decompiled yet (src/stubs/session.cpp, src/stubs/lane_d.cpp) */
void __stdcall function_07b140(void *x, long a, long ten, long twelve, void *local);
void __stdcall function_095580(void *stream, long message_type, long message_size, const void *message);

long network_reliable_stream_allocate(long owner);
long network_stream_allocate(long owner);
bool network_connection_flags_valid(dword flags);
void network_connection_close(s_network_connection *connection, long reason);
void network_connection_dispose(s_network_connection *connection);
void network_connection_reset_timers(s_network_connection *connection);
void network_connection_reset_timer(s_network_connection *connection, long index);
long network_connection_send_capacity(s_network_connection *connection);
void network_connection_update_handshake(s_network_connection *connection);
void network_connection_send_acknowledge(s_network_connection *connection, bool reliable);

static inline s_network_connection *network_connection_get(long index)
{
	return &((s_network_connection *)g_4d87d4)[index];
}

static inline s_network_stream_header *network_reliable_stream_get(long index)
{
	return (s_network_stream_header *)((byte *)g_4d87d8 + index * 0x97c);
}

static inline s_network_stream_header *network_stream_get(long index)
{
	return (s_network_stream_header *)((byte *)g_4d87dc + index * 0x2850);
}

#endif
