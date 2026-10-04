// @flags /O2 /Gr
/* UNKNOWN_08C5B0.CPP: the settings bungie.net keeps for a signed-in
   user; each handler applies one to the user's controller (lane D) */

#include "unknown_11c920.h"
#include "globals.h"

/* what a handler is given */
struct s_bungienet_user_request
{
	byte unknown00[0xe];
	short kind;
	long controller_index;
};

typedef bool (__stdcall *bungienet_user_handler)(s_bungienet_user_request *request);

struct s_bungienet_user_handler
{
	long unknown00;
	bungienet_user_handler handler;
};

/* the controllers' state (g_54e8e0, 0xc70 bytes each); this file reads only
   the voice setting at +0x204 */
struct s_controller_voice_view
{
	byte unknown000[0x204];
	long voice_setting;
	byte unknown208[0xc70 - 0x208];
};


/* a block bungie.net sent: its flags (bit 1: present) and its length */
struct s_bungienet_block_globals
{
	dword flags;
	short length;
};

s_bungienet_block_globals g_479754;
long g_4d8810;

void function_18fe9e(long index);

static inline s_controller_voice_view *controller_voice_view_get(long controller_index)
{
	return &((s_controller_voice_view *)g_54e8e0)[controller_index];
}

// @retail 0x8c5b0
bool __stdcall bungienet_user_voice_reset(s_bungienet_user_request *request)
{
	long controller_index = request->controller_index;
	s_controller_voice_view *controller = controller_voice_view_get(controller_index);
	long old_setting = controller->voice_setting;
	bool changed = false;
	if (old_setting != 1)
		changed = true;
	controller->voice_setting = 1;
	if (changed)
		function_18fe9e(controller_index);
	return true;
}

static inline bool bungienet_block_available(void)
{
	if (g_479754.length != 0 && (g_479754.flags & 2))
		return true;
	return false;
}

static inline long bungienet_user_voice_setting(short kind)
{
	if (kind == 1 && bungienet_block_available())
		return g_4d8810;
	return 1;
}

// @retail 0x8c5f0
bool __stdcall bungienet_user_voice_apply(s_bungienet_user_request *request)
{
	long controller_index = request->controller_index;
	long setting = bungienet_user_voice_setting(request->kind);
	s_controller_voice_view *controller = controller_voice_view_get(controller_index);
	long old_setting = controller->voice_setting;
	bool changed = false;
	if (setting != old_setting)
		changed = true;
	controller->voice_setting = setting;
	if (changed)
		function_18fe9e(controller_index);
	return true;
}

s_bungienet_user_handler g_4672a8[2] =
{
	{ 0, bungienet_user_voice_reset },
	{ 0, bungienet_user_voice_apply },
};