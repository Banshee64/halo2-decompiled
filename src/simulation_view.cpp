// @flags /O2 /Gr
/* SIMULATION_VIEW.CPP: a simulation world's view onto one remote machine:
   its establishment state machine (states 0..5, each with an id, mirrored by
   the remote end), its replication baseline and its buffered join data
   (lane D) */

#include "cseries.h"
#include <xtl.h>
#include <string.h>
#include "globals.h"
#include "network_observer.h"
#include "simulation_world.h"

/* the establishment message (type 0x25) */
struct s_simulation_view_establishment
{
	long state;
	long id;
};

byte g_510ca2;

/* frees a block (not decompiled yet: src/stubs/memory.cpp) */
void function_12d520(long a);

/* the time source; retail inlines network_time_get here */
static inline long view_time_get(void)
{
	if (g_510548)
		return g_51054c;
	return GetTickCount();
}

static inline long view_time_since(long time)
{
	return view_time_get() - time;
}

static inline void view_send_message(c_simulation_view *view, long message_type, long message_size, const void *message)
{
	if (view->channel_index != NONE)
		network_observer_send_message(view->observer, 3, view->channel_index, false, message_type, message_size, message);
}

// @retail 0x859b0
bool c_simulation_view::channel_ready(void)
{
	bool result;
	if (channel_index != NONE)
		result = network_observer_channel_ready(observer, channel_index, 0x2b);
	else
		result = true;
	return result;
}

// @retail 0x85a70
void c_simulation_view::set_state(long new_state, long id)
{
	bool valid;

	if (new_state < 2)
		valid = id == NONE;
	else if (new_state == 2)
		valid = id >= 0;
	else
		valid = id == state_id && new_state == state + 1;

	if (!valid)
	{
		fail(7);
	}
	else if (state != new_state || state_id != id)
	{
		s_simulation_view_establishment message;
		memset(&message, 0, sizeof(message));
		state_id = id;
		message.id = id;
		state = new_state;
		message.state = new_state;
		view_send_message(this, 0x25, sizeof(message), &message);
		update_established();
	}
}

// @retail 0x862c0
void c_simulation_view::fail(long reason)
{
	if (failure_reason == 0)
	{
		set_state(0, NONE);
		failure_reason = reason;
	}
}

// @retail 0x86590
void c_simulation_view::release_buffer(void)
{
	if (buffer)
	{
		unknown9c = 0;
		unknowna0 = 0;
		function_12d520((long)buffer);
		buffer = 0;
		unknown98 = 0;
	}
}

/* the callback a buffer's owner calls when it goes away */
// @retail 0x86aa0
void __stdcall simulation_view_buffer_disposed(byte *buffer, c_simulation_view *view)
{
	if (view->type == 2 && view->buffer && view->buffer == buffer)
		view->release_buffer();
}

// @retail 0x85600
void c_simulation_view::detach(void)
{
	if (type == 2 && buffer)
		release_buffer();
	world->views[world_index] = 0;
	world->view_count--;
	world = 0;
	world_index = NONE;
}

// @retail 0x85540
void c_simulation_view::initialize(long unknown04_, short type_, s_simulation_view_data *data_, const s_machine_address *address_, long unknown1c_)
{
	unknown04 = unknown04_;
	data = data_;
	type = type_;
	address = *address_;
	unknown1c = unknown1c_;
	world = 0;
	world_index = NONE;
	observer = 0;
	channel_index = NONE;
	failure_reason = 0;
	state = 0;
	state_id = NONE;
	remote_state = 0;
	remote_id = NONE;
	unknown3c = NONE;
	unknown40 = NONE;
	flag78 = false;
	if ((1 << type) & 0x14)
		player_mask = 0;
	if (type_ == 2)
	{
		unknown80 = NONE;
		unknown84 = NONE;
		unknown90 = 0;
		buffer = 0;
		unknown98 = 0;
		unknownac = 0;
		unknown88 = false;
	}
	else if (type_ == 1)
	{
		unknownb0 = 0;
	}
}

// @retail 0x86b40
void simulation_view_baseline_set_active(s_simulation_view_baseline *baseline, bool active)
{
	if (active)
	{
		if ((1 << baseline->view->type) & 0x14)
		{
			memset(baseline->state, 0, sizeof(baseline->state));
			baseline->time = NONE;
			baseline->sequence = 0;
			baseline->active = true;
		}
		else
		{
			memset(baseline->state, 0, sizeof(baseline->state));
			baseline->sequence = 0;
			g_510ca2 = true;
			baseline->unknown06 = true;
		}
	}
	else
	{
		if (baseline->active)
			baseline->active = false;
		if (baseline->unknown06)
		{
			g_510ca2 = false;
			baseline->unknown06 = false;
		}
	}
}

// @retail 0x861c0
void c_simulation_view::update_established(void)
{
	bool now_established = false;
	bool synchronized = false;

	if (world && unknown3c != NONE && state_id == remote_id)
	{
		now_established = state >= 2 && remote_state >= 2;
		synchronized = state >= 5 && remote_state >= 5;
	}
	if (now_established != established())
	{
		flag75 = now_established;
		if (!now_established)
		{
			if ((1 << type) & 0x14)
			{
				player_mask = 0;
				if (type == 2)
				{
					unknown80 = NONE;
					unknown84 = NONE;
					if (buffer)
						release_buffer();
				}
			}
			else if (type == 1)
			{
				unknownb0 = 0;
			}
		}
		if (type == 3 || type == 4)
			simulation_view_baseline_set_active(&data->baseline, now_established);
		simulation_world_view_established(world, this, now_established);
	}
	if (flag78 != synchronized)
	{
		flag78 = synchronized;
		simulation_world_view_synchronized(world, this, synchronized);
	}
}

// @retail 0x86140
void c_simulation_view::set_unknown88(bool value)
{
	if (value && !unknown88)
		time8c = view_time_get();
	unknown88 = value;
	if (value && view_time_since(time8c) >= 2000)
		fail(4);
}

// @retail 0x85f50
bool c_simulation_view::handle_player_update(bool failed, long a, long b, dword controller_mask, const s_simulation_player_state *states)
{
	bool result = false;

	if (failed)
	{
		fail(2);
		result = true;
	}
	else if (a >= unknown80 && b >= unknown84 && b < world->unknown28)
	{
		if (world->unknown18 == 4)
		{
			unknown84 = b;
			unknown80 = a;
			function_6a7f0(world, (s_key_450d14 *)&address, controller_mask, states);
		}
		result = true;
	}
	return result;
}
