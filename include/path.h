/* PATH.H: the ai's path search (src/unknown_271e50.cpp) and the query an
   actor builds for it (src/unknown_1f9240.cpp fills its settings, 0x1f90f0
   its source). */

#ifndef PATH_H
#define PATH_H

#include "cseries.h"

/* what an actor may path through (0x1c bytes, function_1f9240) */
struct s_path_settings
{
	long flags;
	long unknown04;
	long unknown08;
	bool unknown0c;
	byte unknown0d;
	short unknown0e;
	short unknown10;
	short unknown12;
	short unknown14;
	byte unknown16[2];
	long unknown18;
};

/* where a path starts (0x50 bytes; function_1f90f0 fills the first 0x44) */
struct s_path_source
{
	byte unknown00[0x44];
	byte unknown44;
	bool unknown45;
	byte unknown46[2];
	real unknown48;
	byte unknown4c[4];
};

struct s_path_query
{
	s_path_settings settings;
	s_path_source source;
};

/* a location a search may begin from (0x1c bytes) */
struct s_path_location
{
	short unknown00;
	short unknown02;
	byte unknown04[0x18];
};

struct path_node
{
	short heap_index;
	byte unknown02[0x42];
};

struct path_heap_entry
{
	short node;
	short cost;
};

struct path_state
{
	s_path_source source;
	long flags;
	byte unknown54;
	byte unknown55[0x70 - 0x55];
	s_path_location location;
	void *pathfinding;
	short unknown90;
	byte unknown92[0xac - 0x92];
	byte unknownac;
	byte unknownad;
	short unknownae;
	byte unknownb0[0xf0 - 0xb0];
	path_node nodes[1023];
	byte unknown_pad[4];
	short heap_count;
	path_heap_entry heap[1024];
	byte unknown120b2[0x140b8 - 0x120b2];
	s_path_settings settings;
	byte unknown140d4[0x14188 - 0x140d4];
	short unknown14188;
};

void function_271300(path_state *state, s_path_location const *location, s_path_settings const *settings,
	s_path_source const *source, long flags);

#endif
