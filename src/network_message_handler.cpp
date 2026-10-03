// @flags /O2 /Gr
/* NETWORK_MESSAGE_HANDLER.CPP: the handler the message gateway (0x93aa0)
   passes each incoming session message to. Every session message begins
   with the identifier of the session it is for; the handler looks the
   session up in the session manager and passes the message on to the
   session code (src/network_session.cpp, lane D). */

#include "cseries.h"
#include <string.h>
#include "globals.h"
#include "network_session.h"
#include "network_connection.h"
#include "transport_address.h"
#include "unknown_058dd0.h"

/* the messages (src/network_session.cpp) */
struct s_network_message_handoff;
struct s_network_message_peer_properties;
struct s_network_message_peer_identity;
struct s_network_message_player_refuse;
struct s_network_message_player_remove;
struct s_network_message_player_properties;
struct s_network_message_parameters_request;
struct s_network_message_countdown_timer;
struct s_network_message_mode_acknowledge;

/* src/network_session.cpp */
long network_session_find_member_by_address(c_network_session *session, const transport_address *address);
bool network_session_channel_is_host(c_network_session *session, long remote_index);
bool network_session_handle_mode_acknowledge(c_network_session *session, const s_network_message_mode_acknowledge *message, long remote_index);
void network_session_close(c_network_session *session);
bool network_session_channel_is_host_address(c_network_session *session, const transport_address *address);
bool network_session_address_is_leaving_host(c_network_session *session, const transport_address *address);
bool network_session_handle_countdown_timer(c_network_session *session, long remote_index, const s_network_message_countdown_timer *message);
bool network_session_handle_session_disband(c_network_session *session, const transport_address *address);
bool network_session_handle_session_boot(c_network_session *session, const transport_address *address);
bool network_session_handle_channel_closed(c_network_session *session, long remote_index);
bool network_session_handle_peer_ready(c_network_session *session, long remote_index);
bool network_session_handle_player_refuse(c_network_session *session, const s_network_message_player_refuse *message, long remote_index);
bool network_session_handle_player_remove(c_network_session *session, long remote_index, const s_network_message_player_remove *message);
bool network_session_handle_peer_reestablish(c_network_session *session, const transport_address *address);
bool network_session_handle_peer_properties(c_network_session *session, long remote_index, const s_network_message_peer_properties *message);
bool network_session_handle_delegate_leader(c_network_session *session, long remote_index, const s_network_message_peer_identity *message);
bool network_session_handle_host_handoff_acknowledge(c_network_session *session, long remote_index, const byte *message);
bool network_session_handle_boot_machine(c_network_session *session, long remote_index, const s_network_message_peer_identity *message);
void network_session_handle_join_abort_reply(c_network_session *session, const transport_address *address);
bool network_session_member_leave(c_network_session *session, long member_index);
bool network_session_handle_parameters_request(const s_network_message_parameters_request *message, c_network_session *session, long remote_index);
bool network_session_handle_host_handoff(c_network_session *session, const s_network_message_handoff *message);
bool network_session_handle_player_properties(c_network_session *session, long remote_index, const s_network_message_player_properties *message);

/* the link's send (src/stubs/session.cpp) */
void __stdcall function_07b140(void *x, long a, long ten, long twelve, void *local);

/* src/network_connection.cpp */
void network_connection_close(s_network_connection *connection, long reason);

/* the sessions the session manager owns */
struct s_network_session_list
{
	c_network_session *sessions[3];
};

class c_network_message_handler
{
public:
	void handle_join_abort(const s_session_id *message, const transport_address *address);
	void handle_leave_session(const s_session_id *message, const transport_address *address);
	void handle_leave_acknowledge(const s_session_id *message, const transport_address *address);
	void handle_session_disband(const s_session_id *message, const transport_address *address);
	void handle_session_boot(const s_session_id *message, const transport_address *address);
	void handle_host_handoff(const s_network_message_handoff *message, long remote_index);
	void handle_host_handoff_acknowledge(const byte *message, long remote_index);
	void handle_channel_closed(const s_session_id *message, long remote_index);
	void handle_peer_ready(const s_session_id *message, long remote_index);
	void handle_peer_reestablish(const s_session_id *message, const transport_address *address);
	void handle_peer_properties(const s_network_message_peer_properties *message, long remote_index);
	void handle_delegate_leader(const s_network_message_peer_identity *message, long remote_index);
	void handle_boot_machine(const s_network_message_peer_identity *message, long remote_index);
	void handle_player_refuse(const s_network_message_player_refuse *message, long remote_index);
	void handle_player_remove(const s_network_message_player_remove *message, long remote_index);
	void handle_player_properties(const s_network_message_player_properties *message, long remote_index);
	void handle_parameters_request(const s_network_message_parameters_request *message, long remote_index);
	void handle_countdown_timer(const s_network_message_countdown_timer *message, long remote_index);
	void handle_mode_acknowledge(const s_network_message_mode_acknowledge *message, long remote_index);

	byte unknown00[0xc];
	void *link;
	byte unknown10[4];
	s_network_session_list *session_manager;
};

// @retail 0x75800
c_network_session *network_session_manager_find_session(s_network_session_list *manager, const s_session_id *session_id)
{
	for (dword i = 0; i < sizeof(manager->sessions) / sizeof(manager->sessions[0]); i++)
	{
		c_network_session *session = manager->sessions[i];
		if (session && session->state != 0 && session->flag24)
		{
			s_session_id id = *(s_session_id *)&session->unknown1c;
			if (memcmp(&id, session_id, sizeof(s_session_id)) == 0)
				return manager->sessions[i];
		}
	}
	return 0;
}

/* the reply to a connect request */
struct s_connect_request
{
	word identifier;
	byte unknown02[2];
	long sequence;
};

struct s_network_message_connect_reply
{
	word identifier;
	byte unknown02[2];
	long sequence;
	long reason;
};

// @retail 0x93f60
void network_message_handler_refuse_connect(const s_connect_request *request, c_network_message_handler *handler, long address)
{
	s_network_message_connect_reply reply;
	reply.identifier = request->identifier;
	reply.sequence = request->sequence;
	reply.reason = 2;
	function_07b140(handler->link, address, 1, sizeof(reply), &reply);
}

/* the reply to a peer leaving a session */
struct s_network_message_leave_acknowledge
{
	s_session_id session_id;
	long reason;
};

// @retail 0x94150
void network_message_handler_handle_leave_request(c_network_message_handler *handler, long address, const s_session_id *message)
{
	c_network_session *session = network_session_manager_find_session(handler->session_manager, message);
	if (session && function_058d70(session) && session->function_058d20())
	{
		long reason;
		long member_index = network_session_find_member_by_address(session, (const transport_address *)address);
		if (member_index == NONE || member_index == session->current_member || network_session_member_leave(session, member_index))
			reason = 8;
		else
			reason = 9;
		s_network_message_leave_acknowledge reply = { 0 };
		reply.session_id = *message;
		reply.reason = reason;
		function_07b140(handler->link, address, 10, sizeof(reply), &reply);
	}
}

/* 0x94220 (handle_join_abort), kept out of the build: it calls network_session_close
   (0x5a520, lane D), and with it as a caller our LTCG passes that function's
   session in ecx instead of eax, which breaks lane D's matched 0x593e0,
   0x618d0 and 0x61910. Not matched itself: 0x5a520's convention differs. */
#if 0
void c_network_message_handler::handle_join_abort(const s_session_id *message, const transport_address *address)
{
	c_network_session *session = network_session_manager_find_session(session_manager, message);
	if (session)
	{
		if (network_session_channel_is_host_address(session, address))
		{
			if (network_session_channel_is_host_address(session, address))
				network_session_close(session);
		}
		else if (network_session_address_is_leaving_host(session, address))
		{
			network_session_handle_join_abort_reply(session, address);
		}
	}
}
#endif

// @retail 0x94270
void c_network_message_handler::handle_leave_session(const s_session_id *message, const transport_address *address)
{
	c_network_session *session = network_session_manager_find_session(session_manager, message);
	if (session && session->function_058d20())
	{
		long member_index = network_session_find_member_by_address(session, address);
		if (member_index != NONE && member_index != session->current_member)
			network_session_member_leave(session, member_index);
	}
}

/* 0x942d0 (handle_leave_acknowledge), kept out of the build: it calls network_session_close
   (0x5a520, lane D), and with it as a caller our LTCG passes that function's
   session in ecx instead of eax, which breaks lane D's matched 0x593e0,
   0x618d0 and 0x61910. Not matched itself: 0x5a520's convention differs. */
#if 0
void c_network_message_handler::handle_leave_acknowledge(const s_session_id *message, const transport_address *address)
{
	c_network_session *session = network_session_manager_find_session(session_manager, message);
	if (session && function_058d90(session) && session->state == 4 &&
		network_session_find_member_by_address(session, address) == session->member_index)
	{
		network_session_close(session);
	}
}
#endif

/* 0x94310 (handle_session_disband), kept out of the build: it matches, but
   as a caller of 0x5e150 it makes our LTCG pass network_session_close's
   (0x5a520) session in ecx instead of eax, which breaks lane D's matched
   0x593e0, 0x618d0 and 0x61910. */
#if 0
void c_network_message_handler::handle_session_disband(const s_session_id *message, const transport_address *address)
{
	c_network_session *session = network_session_manager_find_session(session_manager, message);
	if (session)
		network_session_handle_session_disband(session, address);
}
#endif

/* 0x94330 (handle_session_boot), kept out of the build: it matches, but
   as a caller of 0x5e1a0 it makes our LTCG pass network_session_close's
   (0x5a520) session in ecx instead of eax, which breaks lane D's matched
   0x593e0, 0x618d0 and 0x61910. */
#if 0
void c_network_message_handler::handle_session_boot(const s_session_id *message, const transport_address *address)
{
	c_network_session *session = network_session_manager_find_session(session_manager, message);
	if (session)
		network_session_handle_session_boot(session, address);
}
#endif

// @retail 0x94350
void c_network_message_handler::handle_host_handoff(const s_network_message_handoff *message, long remote_index)
{
	c_network_session *session = network_session_manager_find_session(session_manager, (const s_session_id *)message);
	if (session && network_session_channel_is_host(session, remote_index))
		network_session_handle_host_handoff(session, message);
}

// @retail 0x94380
void c_network_message_handler::handle_host_handoff_acknowledge(const byte *message, long remote_index)
{
	c_network_session *session = network_session_manager_find_session(session_manager, (const s_session_id *)message);
	if (session && function_058d70(session) && session->function_058d20())
		network_session_handle_host_handoff_acknowledge(session, remote_index, message);
}

// @retail 0x94440
void c_network_message_handler::handle_channel_closed(const s_session_id *message, long remote_index)
{
	c_network_session *session = network_session_manager_find_session(session_manager, message);
	if (session)
		network_session_handle_channel_closed(session, remote_index);
}

// @retail 0x94460
void c_network_message_handler::handle_peer_ready(const s_session_id *message, long remote_index)
{
	c_network_session *session = network_session_manager_find_session(session_manager, message);
	if (session && session->function_058d20())
		network_session_handle_peer_ready(session, remote_index);
}

// @retail 0x945d0
void c_network_message_handler::handle_peer_reestablish(const s_session_id *message, const transport_address *address)
{
	c_network_session *session = network_session_manager_find_session(session_manager, message);
	if (session)
		network_session_handle_peer_reestablish(session, address);
}

// @retail 0x94640
void c_network_message_handler::handle_peer_properties(const s_network_message_peer_properties *message, long remote_index)
{
	c_network_session *session = network_session_manager_find_session(session_manager, (const s_session_id *)message);
	if (session && session->function_058d20())
		network_session_handle_peer_properties(session, remote_index, message);
}

// @retail 0x94680
void c_network_message_handler::handle_delegate_leader(const s_network_message_peer_identity *message, long remote_index)
{
	c_network_session *session = network_session_manager_find_session(session_manager, (const s_session_id *)message);
	if (session && session->function_058d20())
		network_session_handle_delegate_leader(session, remote_index, message);
}

// @retail 0x946c0
void c_network_message_handler::handle_boot_machine(const s_network_message_peer_identity *message, long remote_index)
{
	c_network_session *session = network_session_manager_find_session(session_manager, (const s_session_id *)message);
	if (session && session->function_058d20())
		network_session_handle_boot_machine(session, remote_index, message);
}

// @retail 0x94740
void c_network_message_handler::handle_player_refuse(const s_network_message_player_refuse *message, long remote_index)
{
	c_network_session *session = network_session_manager_find_session(session_manager, (const s_session_id *)message);
	if (session && network_session_channel_is_host(session, remote_index))
		network_session_handle_player_refuse(session, message, remote_index);
}

// @retail 0x94780
void c_network_message_handler::handle_player_remove(const s_network_message_player_remove *message, long remote_index)
{
	c_network_session *session = network_session_manager_find_session(session_manager, (const s_session_id *)message);
	if (session && session->function_058d20())
		network_session_handle_player_remove(session, remote_index, message);
}

// @retail 0x947c0
void c_network_message_handler::handle_player_properties(const s_network_message_player_properties *message, long remote_index)
{
	c_network_session *session = network_session_manager_find_session(session_manager, (const s_session_id *)message);
	if (session && session->function_058d20())
		network_session_handle_player_properties(session, remote_index, message);
}

// @retail 0x94830
void c_network_message_handler::handle_parameters_request(const s_network_message_parameters_request *message, long remote_index)
{
	c_network_session *session = network_session_manager_find_session(session_manager, (const s_session_id *)message);
	if (session && session->function_058d20())
		network_session_handle_parameters_request(message, session, remote_index);
}

// @retail 0x94870
void c_network_message_handler::handle_countdown_timer(const s_network_message_countdown_timer *message, long remote_index)
{
	c_network_session *session = network_session_manager_find_session(session_manager, (const s_session_id *)message);
	if (session && session->function_058d20())
		network_session_handle_countdown_timer(session, remote_index, message);
}

// @retail 0x948c0
void c_network_message_handler::handle_mode_acknowledge(const s_network_message_mode_acknowledge *message, long remote_index)
{
	c_network_session *session = network_session_manager_find_session(session_manager, (const s_session_id *)message);
	if (session && function_058d70(session) && session->function_058d20())
		network_session_handle_mode_acknowledge(session, message, remote_index);
}

/* the connection messages: the identifier of the connection and, for a
   close, its reason */
struct s_network_message_connect_refuse
{
	long identifier;
	long reason;
};

struct s_network_message_connect_closed
{
	long identifier;
};

#define CONNECTION(index) (&((s_network_connection *)g_4d87d4)[index])

// @retail 0x94ae0
void network_message_handle_connect_refuse(long connection_index, const s_network_message_connect_refuse *message)
{
	s_network_connection *connection = CONNECTION(connection_index);
	long state = connection->state;
	if (state > 2 && state != 5 && connection->local_sequence == message->identifier)
	{
		long reason = message->reason;
		switch (reason)
		{
		case 0:
		case 1:
		case 2:
		case 5:
		case 6:
			network_connection_close(connection, 5);
			break;
		case 3:
		case 4:
		case 7:
			break;
		default:
			__assume(0);
		}
	}
}

// @retail 0x94bb0
void network_message_handle_connect_closed(long connection_index, const s_network_message_connect_closed *message)
{
	s_network_connection *connection = CONNECTION(connection_index);
	if (connection->state > 2 && connection->local_sequence == message->identifier)
		network_connection_close(connection, 10);
}
