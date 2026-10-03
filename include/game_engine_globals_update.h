/* GAME_ENGINE_GLOBALS_UPDATE.H: the fields every game engine's globals
   entity updates (src/unknown_0a4ab0.cpp) */

#ifndef GAME_ENGINE_GLOBALS_UPDATE_H
#define GAME_ENGINE_GLOBALS_UPDATE_H

#include "cseries.h"
#include "bitstream.h"

struct s_game_engine_globals_update
{
	word team_mapping0;
	word team_mask;
	word team_mapping4;
	word team_mapping6;
	word team_mapping8;
	short team_indices[9];
	byte current_state;
	bool game_finished;
	short current_round;
	short round_timer;
};

bool game_engine_globals_write_update(long reserve_bits, dword requested, dword *written,
	s_game_engine_globals_update const *update, s_bitstream *stream);
bool game_engine_globals_read_update(s_bitstream *stream, s_game_engine_globals_update *update, dword *read);

#endif
