#include "unknown_11c920.h"
#include "effects.h"

// @flags /O2 /arch:SSE /Gr

struct s_247fd0
{
	real field_00[17];
	dword field_44;
	void *field_48;
	void *field_4c;
	void const *field_50;
};

struct s_247fd3
{
	word field_00;
	word field_02;
	long field_04;
};

struct s_248000
{
	word field_00;
	union
	{
		word field_02;
		struct
		{
			word : 13;
			word field_02_13 : 3;
		};
	};
	long field_04;
	real field_08;
	real field_0c;
	real field_10;
	word field_14;
	word field_16;
	word field_18;
	word field_1a;
	point3f field_1c;
	vector3f field_28;
	real field_34;
	real field_38;
	dword field_3c;
};

class c_248228
{
public:
	virtual void function_248228() = 0;
	virtual void function_248229() = 0;
	virtual void function_24822a() = 0;
	virtual void function_24822b() = 0;
	virtual void function_24822c() = 0;
	virtual void function_24822d() = 0;
	virtual long function_24822e() = 0;
	virtual s_effect_particle_system_definition *function_24822f(word arg_0) = 0;
};

struct s_particle_spawn_definition;
struct s_particle_spawn_state;
void function_249aa0(s_particle_spawn_definition const *arg_0, s_particle_spawn_state *arg_1,
	point3f const *arg_2, real const *arg_3, vector3f const *arg_4);
void __stdcall function_173ba0(dword arg_0, void *arg_1, void *arg_2, void const *arg_3, real *arg_4);

PRIVATE __forceinline void function_248076(dword *arg_0, real &arg_1)
{
	arg_1 = function_x82e52f(arg_0, 0, 0);
}

// @retail 0x247fd0
void function_247fd0(s_247fd3 *arg_1, s_particle_system_datum *arg_2,
	point3f const *arg_3, vector3f const *arg_4, dword arg_5, s_247fd0 *arg_0)
{
	long local_0 = record_pool_allocate(g_51ec84);
	if (local_0 != NONE)
	{
		s_248000 *local_1 = &((s_248000 *)g_51ec84->data)[local_0 & 0xffff];
		s_effect_particle_system_definition *local_2 = arg_2->function_1751d0();
		s_particle_spawn_definition const *local_3 = *(s_particle_spawn_definition const **)local_2->unknown34;
		local_1->field_04 = arg_1->field_04;
		arg_1->field_04 = local_0;
		local_1->field_02 &= 0xe004;
		local_1->field_02 |= 4;
		local_1->field_02_13 = (_random(&g_4e7408->seed, 0, 0) * 5) >> 16;
		local_1->field_08 = 0.f;
		real local_9;
		function_248076(&g_4e7408->seed, local_9);
		local_1->field_14 = (word)real_truncate(local_9 * 65535.f);
		function_248076(&g_4e7408->seed, local_9);
		local_1->field_16 = (word)real_truncate(local_9 * 65535.f);
		function_248076(&g_4e7408->seed, local_9);
		local_1->field_18 = (word)real_truncate(local_9 * 65535.f);
		function_248076(&g_4e7408->seed, local_9);
		local_1->field_1a = (word)real_truncate(local_9 * 65535.f);
		local_1->field_10 = arg_2->unknown04;
		local_1->field_38 = function_x82e52f(&g_4e7408->seed, 0, 0);
		if (arg_0->field_50 != local_1)
		{
			arg_0->field_44 &= ~0xf80f;
			arg_0->field_50 = local_1;
		}
		function_173ba0(~arg_0->field_44 & 0xf80f, arg_0->field_48, arg_0->field_4c, arg_0->field_50, arg_0->field_00);
		arg_0->field_44 |= 0xf80f;
		function_249aa0(local_3, (s_particle_spawn_state *)local_1, arg_3, arg_0->field_00, arg_4);
		local_1->field_3c = arg_5;
		++arg_1->field_02;
		c_248228 *local_4 = (c_248228 *)function_137bd0(local_2->tag_index);
		for (long local_5 = 0; local_5 < local_4->function_24822e(); ++local_5)
		{
			long local_6 = function_173fd0(local_4->function_24822f((word)local_5), arg_2->effect_index,
				local_2->tag_index, (short)local_5, NONE);
			local_4 = (c_248228 *)function_137bd0(local_2->tag_index);
			if (local_6 != NONE)
			{
				s_particle_system_datum *local_7 = &((s_particle_system_datum *)g_510c74->data)[local_6 & 0xffff];
				particle_system_link(local_7, &arg_2->last_child_index, &arg_2->first_child_index);
				local_7->parent = arg_2;
				local_7->unknown4c = local_0;
				real local_8 = local_1->field_0c > 0.f ? 1.f / local_1->field_0c : 0.f;
				local_7->flag0 = true;
				local_7->unknown04 = 0.f;
				local_7->unknown08 = local_8 > 0.f ? 1.f / local_8 : 1.f;
			}
		}
	}
}

struct s_248cc0
{
	long field_00;
	long field_04;
	long field_08;
};

struct s_248ce2
{
	short field_00;
	word field_02;
	long field_04;
	long field_08;
	real field_0c;
	byte field_10[0x4c - 0x10];
};

struct s_particle_emitter_datum;
void function_2483f0(s_particle_emitter_datum *arg_0, long *arg_1, long *arg_2);

// @retail 0x248cc0
void function_248cc0(s_248cc0 *arg_0, s_247fd0 *arg_1, s_particle_system_datum *arg_2,
	point3f const *arg_3, vector3f const *arg_4)
{
	s_record_pool *local_0 = g_51ec88;
	if (arg_0->field_04 == NONE)
	{
		long local_1 = record_pool_allocate(local_0);
		if (local_1 == NONE)
			return;
		s_248ce2 *local_2 = &((s_248ce2 *)local_0->data)[local_1 & 0xffff];
		local_2->field_08 = NONE;
		local_2->field_02 = 0;
		local_2->field_04 = NONE;
		local_2->field_0c = 0.f;
		function_2483f0((s_particle_emitter_datum *)&((s_248ce2 *)local_0->data)[local_1 & 0xffff], &arg_0->field_04, &arg_0->field_08);
	}
	if (arg_0->field_04 != NONE)
		function_247fd0((s_247fd3 *)&((s_248ce2 *)local_0->data)[arg_0->field_04 & 0xffff], arg_2, arg_3, arg_4, arg_2->color, arg_1);
}
