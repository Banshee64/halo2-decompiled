// @flags /O2 /Gr
#include "cseries.h"

// External manager object (declarations only); slots at vtable +0x9c, +0xb0, +0xb4, +0xb8.
class c_manager_interface
{
public:
	virtual long v0();
	virtual long v1();
	virtual long v2();
	virtual long v3();
	virtual long v4();
	virtual long v5();
	virtual long v6();
	virtual long v7();
	virtual long v8();
	virtual long v9();
	virtual long v10();
	virtual long v11();
	virtual long v12();
	virtual long v13();
	virtual long v14();
	virtual long v15();
	virtual long v16();
	virtual long v17();
	virtual long v18();
	virtual long v19();
	virtual long v20();
	virtual long v21();
	virtual long v22();
	virtual long v23();
	virtual long v24();
	virtual long v25();
	virtual long v26();
	virtual long v27();
	virtual long v28();
	virtual long v29();
	virtual long v30();
	virtual long v31();
	virtual long v32();
	virtual long v33();
	virtual long v34();
	virtual long v35();
	virtual long v36();
	virtual long v37();
	virtual long v38();
	virtual long get_current_id();
	virtual long v40();
	virtual long v41();
	virtual long v42();
	virtual long v43();
	virtual long v44(long a, long b);
	virtual long v45(long a, long b, long c);
	virtual bool v46(long a, long b, long c);
};

struct s_globals_4e9ae8
{
	byte unknown00[0x24];
	dword value24;
	byte unknown28[0xc14 - 0x28];
	long manager_index;
};

s_globals_4e9ae8 *g_4e9ae8;
c_manager_interface *g_55e4d0[1];

class c_handler
{
public:
	virtual long get_id() { return 0; }
	virtual bool handler1(long a);
	virtual bool handler2(long a, long b, long c, long d);
	virtual bool handler3(long a, long b, long c, long d);
	virtual void handler4(dword *a);
	virtual bool handler5(long a, long b, long c, long d);
	virtual bool handler6(dword *a);
};

// @retail 0xa45d0
bool c_handler::handler1(long a)
{
	return true;
}

// @retail 0xa45e0
bool c_handler::handler2(long a, long b, long c, long d)
{
	bool result = false;
	c_manager_interface *manager = g_55e4d0[g_4e9ae8->manager_index];
	long id = NONE;
	if (manager)
		id = manager->get_current_id();
	if (get_id() == id)
	{
		g_55e4d0[g_4e9ae8->manager_index]->v44(c, d);
		result = true;
	}
	return result;
}

// @retail 0xa4650
bool c_handler::handler3(long a, long b, long c, long d)
{
	bool result = false;
	c_manager_interface *manager = g_55e4d0[g_4e9ae8->manager_index];
	long id = NONE;
	if (manager)
		id = manager->get_current_id();
	if (get_id() == id)
	{
		g_55e4d0[g_4e9ae8->manager_index]->v45(b, c, d);
		result = true;
	}
	return result;
}

// @retail 0xa46c0
void c_handler::handler4(dword *a)
{
	c_manager_interface *manager = g_55e4d0[g_4e9ae8->manager_index];
	long id = NONE;
	if (manager)
		id = manager->get_current_id();
	if (get_id() == id)
	{
		if (g_55e4d0[g_4e9ae8->manager_index] && g_4e9ae8->value24 != NONE)
		{
			if (((*a ^ g_4e9ae8->value24) & 0x3ff) == 0)
				g_4e9ae8->value24 = *a;
		}
	}
}

// @retail 0xa4730
bool c_handler::handler5(long a, long b, long c, long d)
{
	bool result = false;
	c_manager_interface *manager = g_55e4d0[g_4e9ae8->manager_index];
	long id = NONE;
	if (manager)
		id = manager->get_current_id();
	if (get_id() == id)
		result = g_55e4d0[g_4e9ae8->manager_index]->v46(b, c, d);
	return result;
}

// @retail 0xa47a0
bool c_handler::handler6(dword *a)
{
	bool result = false;
	c_manager_interface *manager = g_55e4d0[g_4e9ae8->manager_index];
	long id = NONE;
	if (manager)
		id = manager->get_current_id();
	if (get_id() == id)
	{
		g_4e9ae8->value24 = *a;
		result = true;
	}
	return result;
}
