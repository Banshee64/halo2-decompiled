// @flags /O2 /arch:SSE /Gr
/* GAME_ENGINE_ENTITY_DEFINITIONS.CPP: the simulation entity definitions of the
   game engine globals (slayer, ctf, oddball, king, territories and juggernaut,
   vtables 0x450fa0, 0x451058, 0x451188, 0x451240, 0x451318 and 0x451410) and
   of the game engine statborg (vtable 0x4516a8).

   The entity definitions have 27 slots. Slots shared with other vtables
   (identical code the linker folded) are owned by the class listed first;
   slots that no decompiled function owns keep placeholder bodies. */

#include "cseries.h"
#include "globals.h"
#include "engine_peer.h"
#include "object_types_21_1.h"
#include "bitstream.h"
#include "flags_writer.h"
#include "game_engine_globals_update.h"
#include <string.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

class c_game_engine_entity_definition;
long function_a4950(c_game_engine_entity_definition const *definition, dword *flags_pointer);
void __stdcall function_1e98e0(void *statborg, long b, void *data);
bool __stdcall function_1e9ad0(void *statborg, long b, void *data);

/* the multiplayer globals, as the statborg sees them */
struct s_statborg_globals_view
{
	byte unknown000[0x304];
	byte statborg[1];
};

class c_game_engine_entity_definition
{
public:
	virtual long v0() { return 0; }
	virtual const char *v1() { return 0; }
	virtual long v2() { return 0; }
	virtual long v3() { return 0; }
	virtual long v4() { return 0; }
	virtual long v5() { return 0; }
	virtual bool v6(long a) { return false; }
	virtual bool v7(long a) { return false; }
	virtual bool v8(long a, long b) { return false; }
	virtual void v9(long a, long b, long *size) {}
	virtual void v10(s_creation_request *request, long parameter, long size, char *buffer) {}
	virtual void v11(long a, dword *flags, long *size) {}
	virtual void v12(long a, void const *data, long c, s_bitstream *stream) {}
	virtual bool v13(long a, void *data, s_bitstream *stream) { return false; }
	virtual bool v14(long a1, long a2, long a3, long a4, long a5, long a6, long a7, long a8) { return false; }
	virtual bool v15(long a, dword *flags, long c, void *data, s_bitstream *stream) { return false; }
	virtual void v16(long a, long b, long c) {}
	virtual bool v17(long a, long b, long c) { return false; }
	virtual void v18(s_entity_slot *entity, long b, void *state) {}
	virtual bool v19(long a, long b, long c, void *data) { return false; }
	virtual bool v20(s_entity_slot *entity, long b, long c, void *data) { return false; }
	virtual void v21(s_entity_slot *entity) {}
	virtual bool v22(s_entity_slot *entity, long b, long c, long d, long e, long f) { return false; }
	virtual bool v23(s_entity_slot *entity, long b, long c, void *data) { return false; }
	virtual bool v24(s_entity_slot *entity) { return false; }
	virtual bool v25(s_entity_slot *entity) { return false; }
	virtual void v26(long a, dword *flags, long size, char *buffer) {}
};

/* slayer: also the oddball's slots 5, 11, 14 and 15 */
class c_slayer_globals_entity_definition : public c_game_engine_entity_definition
{
public:
	virtual long v5();
	virtual void v11(long a, dword *flags, long *size);
	virtual bool v14(long a1, long a2, long a3, long a4, long a5, long a6, long a7, long a8);
	virtual bool v15(long a, dword *flags, long c, void *data, s_bitstream *stream);
};

class c_ctf_globals_entity_definition : public c_game_engine_entity_definition
{
public:
	virtual const char *v1();
	virtual long v2();
	virtual void v11(long a, dword *flags, long *size);
};

class c_oddball_globals_entity_definition : public c_game_engine_entity_definition
{
public:
	virtual const char *v1();
};

class c_king_globals_entity_definition : public c_game_engine_entity_definition
{
public:
	virtual const char *v1();
	virtual long v2();
	virtual long v5();
	virtual void v11(long a, dword *flags, long *size);
};

class c_territories_globals_entity_definition : public c_game_engine_entity_definition
{
public:
	virtual const char *v1();
	virtual long v2();
	virtual long v5();
	virtual void v11(long a, dword *flags, long *size);
};

class c_juggernaut_globals_entity_definition : public c_game_engine_entity_definition
{
public:
	virtual const char *v1();
	virtual long v2();
	virtual void v11(long a, dword *flags, long *size);
};

class c_game_engine_statborg_entity_definition : public c_game_engine_entity_definition
{
public:
	virtual const char *v1();
	virtual long v2();
	virtual void v10(s_creation_request *request, long parameter, long size, char *buffer);
	virtual void v11(long a, dword *flags, long *size);
	virtual bool v14(long a1, long a2, long a3, long a4, long a5, long a6, long a7, long a8);
	virtual bool v15(long a, dword *flags, long c, void *data, s_bitstream *stream);
	virtual bool v19(long a, long b, long c, void *data);
	virtual bool v20(s_entity_slot *entity, long b, long c, void *data);
	virtual void v21(s_entity_slot *entity);
	virtual bool v22(s_entity_slot *entity, long b, long c, long d, long e, long f);
	virtual bool v23(s_entity_slot *entity, long b, long c, void *data);
	virtual bool v24(s_entity_slot *entity);
};

// ---- slayer ----

// @retail 0x9a140
long c_slayer_globals_entity_definition::v5()
{
	return 0x1f;
}

// @retail 0x9a150
void c_slayer_globals_entity_definition::v11(long a, dword *flags, long *size)
{
	*size = function_a4950(this, flags);
}

// @retail 0x9a200
bool c_slayer_globals_entity_definition::v14(long a1, long a2, long a3, long a4, long a5, long a6, long a7, long a8)
{
	return game_engine_globals_write_update(a8, a2, (dword *)a3, (s_game_engine_globals_update const *)a5, (s_bitstream *)a7);
}

// @retail 0x9a230
bool c_slayer_globals_entity_definition::v15(long a, dword *flags, long c, void *data, s_bitstream *stream)
{
	dword read = 0;
	bool result = false;
	if (game_engine_globals_read_update(stream, (s_game_engine_globals_update *)data, &read) && read)
		result = true;
	*flags = read;
	return result;
}

// ---- ctf ----

// @retail 0x998c0
const char *c_ctf_globals_entity_definition::v1()
{
	return "ctf-engine-globals";
}

// @retail 0x998d0
long c_ctf_globals_entity_definition::v2()
{
	return 0x84;
}

// @retail 0x998e0
void c_ctf_globals_entity_definition::v11(long a, dword *flags, long *size)
{
	dword update_flags = *flags;
	long result = function_a4950(this, flags) + 6;
	if (update_flags & 0x20)
		result = MIN(result, 0xf);
	if (update_flags & 0x40)
		result = MIN(result, 0x1b);
	if (update_flags & 0x80)
		result = MIN(result, 0x53);
	if (update_flags & 0x100)
		result = MIN(result, 0x5c);
	if (update_flags & 0x200)
		result = MIN(result, 0x2f);
	if (update_flags & 0x400)
		result = MIN(result, 0x38);
	*size = result;
}

// ---- oddball ----

// @retail 0x9a130
const char *c_oddball_globals_entity_definition::v1()
{
	return "oddball-engine-globals";
}

// ---- king ----

// @retail 0x9a290
const char *c_king_globals_entity_definition::v1()
{
	return "king-engine-globals";
}

// @retail 0x9a2a0
long c_king_globals_entity_definition::v2()
{
	return 0x28;
}

// @retail 0x9a2b0
long c_king_globals_entity_definition::v5()
{
	return 0x7f;
}

// @retail 0x9a2c0
void c_king_globals_entity_definition::v11(long a, dword *flags, long *size)
{
	dword update_flags = *flags;
	long result = function_a4950(this, flags) + 2;
	if (update_flags & 0x20)
		result = MIN(result, 0xb);
	if (update_flags & 0x40)
		result = MIN(result, 0x17);
	*size = result;
}

// ---- territories ----

// @retail 0x9a5c0
const char *c_territories_globals_entity_definition::v1()
{
	return "territories-engine-globals";
}

// @retail 0x9a5d0
long c_territories_globals_entity_definition::v2()
{
	return 0x64;
}

// @retail 0x9a5f0
long c_territories_globals_entity_definition::v5()
{
	return 0x3fffff;
}

// @retail 0x9a600
void c_territories_globals_entity_definition::v11(long a, dword *flags, long *size)
{
	dword update_flags = *flags;
	long result = function_a4950(this, flags) + 0x11;
	if (update_flags & 0x20)
		result = MIN(result, 0x3e);
	for (long i = 0; i < 16; i++)
	{
		if (update_flags & (1 << (i + 6)))
			result = MIN(result, 0x20);
	}
	*size = result;
}

// ---- juggernaut ----

// @retail 0x9a9d0
const char *c_juggernaut_globals_entity_definition::v1()
{
	return "juggernaut-engine-globals";
}

// @retail 0x9a9e0
long c_juggernaut_globals_entity_definition::v2()
{
	return 0x26;
}

// @retail 0x9aa00
void c_juggernaut_globals_entity_definition::v11(long a, dword *flags, long *size)
{
	long result = function_a4950(this, flags) + 1;
	if (*flags & 0x20)
		result = MIN(result, 0x16);
	*size = result;
}

// ---- the game engine statborg ----

/* the statborg of the multiplayer globals, or none without a game engine */
static void *statborg_get()
{
	s_statborg_globals_view *globals = (s_statborg_globals_view *)g_4e9ae8;
	void *statborg = 0;
	if (g_55e4d0[g_4e9ae8->engine_index])
		statborg = globals->statborg;
	return statborg;
}

// @retail 0x9b920
const char *c_game_engine_statborg_entity_definition::v1()
{
	return "game-engine-statborg";
}

// @retail 0x9b930
long c_game_engine_statborg_entity_definition::v2()
{
	return 0x1b0;
}

// @retail 0x9b960
void c_game_engine_statborg_entity_definition::v10(s_creation_request *request, long parameter, long size, char *buffer)
{
	real relevance = -1.0f;
	s_creation_weight *entry = &g_4cef68[request->definition_index];
	if (!(entry->weight > g_45dbd8))
		relevance = function_aa4d0(1, request, entry->field4, parameter, 0);
	csnprintf(buffer, size, "statborg creation: relevance=%5.3f", relevance);
}

// @retail 0x9b9d0
void c_game_engine_statborg_entity_definition::v11(long a, dword *flags, long *size)
{
	dword update_flags = *flags;
	long result = 0x18;
	long i;
	for (i = 0; i < 16; i++)
	{
		if (update_flags & (1 << i))
			result = MIN(result, 0x12);
	}
	for (i = 0; i < 8; i++)
	{
		if (update_flags & (1 << (i + 16)))
			result = MIN(result, 0x12);
	}
	*size = result;
}

// @retail 0x9bb50
bool c_game_engine_statborg_entity_definition::v19(long a, long b, long c, void *data)
{
	memset(data, 0, 0x1b0);
	return true;
}

// @retail 0x9bb70
bool c_game_engine_statborg_entity_definition::v20(s_entity_slot *entity, long b, long c, void *data)
{
	function_1e98e0(statborg_get(), b, data);
	return true;
}

// @retail 0x9bb10
void c_game_engine_statborg_entity_definition::v21(s_entity_slot *entity)
{
	if (g_55e4d0[g_4e9ae8->engine_index] && g_4e9ae8->value28 != NONE)
	{
		if (((entity->id ^ g_4e9ae8->value28) & 0x3ff) == 0)
			g_4e9ae8->value28 = entity->id;
	}
}

// @retail 0x9ba90
bool c_game_engine_statborg_entity_definition::v22(s_entity_slot *entity, long b, long c, long d, long e, long f)
{
	s_mp_globals *globals = g_4e9ae8;
	if (g_55e4d0[globals->engine_index] && globals->value28 != NONE)
		globals->value28 = NONE;
	globals->value28 = entity->id;
	return true;
}

// @retail 0x9bbb0
bool c_game_engine_statborg_entity_definition::v23(s_entity_slot *entity, long b, long c, void *data)
{
	bool result = false;
	if (function_1e9ad0(statborg_get(), b, data))
		result = true;
	return result;
}

// @retail 0x9bad0
bool c_game_engine_statborg_entity_definition::v24(s_entity_slot *entity)
{
	long id = NONE;
	bool result = false;
	if (g_55e4d0[g_4e9ae8->engine_index])
		id = g_4e9ae8->value28;
	if (entity->id == id)
	{
		g_4e9ae8->value28 = NONE;
		result = true;
	}
	return result;
}

/* the statborg's statistics: nine per player and per team */
struct s_statborg_statistics
{
	short values[9];
};

struct s_statborg_data
{
	s_statborg_statistics players[16];
	s_statborg_statistics teams[8];
};

/* writes a signed value of the given size */
static inline void stream_write_signed(s_bitstream *stream, dword value, long size)
{
	function_195720(stream, value & ((1 << size) - 1), size);
}

// @retail 0x9bbf0
bool c_game_engine_statborg_entity_definition::v14(long a1, long a2, long a3, long a4, long a5, long a6, long a7, long a8)
{
	s_statborg_data const *statistics = (s_statborg_data const *)a5;
	s_bitstream *stream = (s_bitstream *)a7;
	s_flags_writer writer;
	flags_writer_initialize(&writer, stream, 0x18, a2, a8);
	bool result = false;
	if (writer.space)
	{
		long i;
		for (i = 0; i < 16; i++)
		{
			if (flags_writer_begin(&writer, "player-update-exists", i))
			{
				for (long j = 0; j < 9; j++)
					stream_write_signed(stream, (word)statistics->players[i].values[j], 16);
			}
			flags_writer_end(&writer);
		}
		for (i = 0; i < 8; i++)
		{
			if (flags_writer_begin(&writer, "team-update-exists", i + 16))
			{
				for (long j = 0; j < 9; j++)
					stream_write_signed(stream, (word)statistics->teams[i].values[j], 16);
			}
			flags_writer_end(&writer);
		}
		*(dword *)a3 |= writer.written;
		result = true;
	}
	return result;
}

/* reads a signed value of the given size */
static inline long stream_read_signed(s_bitstream *stream, long size)
{
	long value = function_1959c0(stream, size);
	if (value & (1 << (size - 1)))
		value |= ~((1 << size) - 1);
	return value;
}

// @retail 0x9bd40
bool c_game_engine_statborg_entity_definition::v15(long a, dword *flags, long c, void *data, s_bitstream *stream)
{
	s_statborg_data *statistics = (s_statborg_data *)data;
	dword mask = 0;
	long i;
	for (i = 0; i < 16; i++)
	{
		if (stream_read_bit(stream))
		{
			for (long j = 0; j < 9; j++)
				statistics->players[i].values[j] = (short)stream_read_signed(stream, 16);
			mask |= 1 << i;
		}
	}
	for (i = 0; i < 8; i++)
	{
		if (stream_read_bit(stream))
		{
			for (long j = 0; j < 9; j++)
				statistics->teams[i].values[j] = (short)stream_read_signed(stream, 16);
			mask |= 1 << (i + 16);
		}
	}
	return (*flags = mask) != 0;
}
