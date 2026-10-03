/* UNKNOWN_03D380.H: callees of batch 18-4 that are not decompiled yet (stubbed
   in src/stubs/unknown_03d380.cpp) */

#ifndef UNKNOWN_03D380_H
#define UNKNOWN_03D380_H

#include "cseries.h"
#include "data_array.h"
#include "object_queries.h"

struct c_simulation_world
{
	void delete_all_players(void);
};

struct s_47f048_object
{
	void function_30be40(long value);
};

void function_593e0(void);
void function_67f60(void);
void function_67ee0(void);
void function_6b040(void);
void function_bb7f0(void);
void function_183f10(void);
void function_162420(void);
void __stdcall function_162060(void *p);
void __stdcall function_83370(void *a, dword b);
void __stdcall function_6a770(void *a);
void __stdcall function_127320(long datum, long count);
void function_21d4d0(void);
void function_21f290(void);
void function_125d60(void);
void function_21a1e0(void);
void __stdcall function_23654b(void *c, void *a, void *b);
void __stdcall function_152f80(void *a, void *c);
void function_155380(void);
void __stdcall function_155a30(byte value);
void __stdcall function_3e2ff0(void *p);
void function_1565e0(void);
void function_11bed0(real_point3d const *point, s_location *location);
void __stdcall function_16f4b0(void *player);
long __stdcall function_18d1c0(long value);
bool __fastcall function_18d360(long value);
void function_18d290(long looping_sound_index, long tag_index);
long __stdcall function_122c70(void *iterator);
struct s_sound_driver_volumes;
void function_220fd0(s_sound_driver_volumes const *volumes);
void function_225ab0(void);
void __stdcall function_18bb80(real value);
void function_1c2b10(void);
void function_1c29d0(void);
void function_1c2890(void);
void function_1c2910(void);
void __stdcall function_1c39c0(void *p, long value);
void function_1c2a10(void);
void __stdcall function_1e75d0(dword value);
void __stdcall function_72c70(dword value);
void function_43820(void);
void function_43850(void);
long __stdcall function_25dd20(long key);
bool __stdcall function_25dd30(long a, long b);
void __fastcall scripted_hud_messages_clear(void);

#endif
