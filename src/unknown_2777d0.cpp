// @flags /O2 /Gr
#include "unknown_11c920.h"
#include "globals.h"
#include "havok_reference.h"

/* Interfaces whose destructors restore their retail virtual tables. */
class c_interface_2777d0
{
public:
	virtual ~c_interface_2777d0();
	virtual void slot1() { }
	virtual void slot2() { }
};

class c_interface_2777f0
{
public:
	virtual ~c_interface_2777f0();
	virtual void slot1() { }
	virtual void slot2() { }
	virtual void slot3() { }
	virtual void slot4() { }
	virtual void slot5() { }
	virtual long slot6() { return 0; }
};

class c_interface_278b40
{
public:
	virtual ~c_interface_278b40();
	virtual void slot1() { }
	virtual void slot2() { }
};

// @retail 0x2777d0 deleting
c_interface_2777d0::~c_interface_2777d0()
{
}

// @retail 0x277800 deleting c_interface_2777f0
// @retail 0x2777f0
c_interface_2777f0::~c_interface_2777f0()
{
}

// @retail 0x278b40 deleting
c_interface_278b40::~c_interface_278b40()
{
}

struct s_277890_buffer
{
	long *data;
	long count;
	long capacity;

	~s_277890_buffer()
	{
		if (!(capacity & 0x80000000))
			g_480118->allocate((long)data, (capacity & 0x7fffffff) * sizeof(long), 0x12);
	}
};

class c_interface_277890 : public c_interface_2777d0
{
public:
	long unknown04;
	s_277890_buffer buffer;
	byte unknown14[8];
	c_havok_reference_counted reference;

};

// @retail 0x277890 deleting c_interface_277890

long function_147090(void);
void *g_55e508;
void *g_55e50c;
long g_55e504;
long g_55e500;
bool g_55e4fc;

struct s_278e80_state
{
	byte unknown00[0xae];
	bool changed;
};

// @retail 0x278e60
void __cdecl function_278e60(void *context)
{
	g_55e508 = context;
	g_55e504 = function_147090();
}

// @retail 0x278e80
void __cdecl function_278e80(s_278e80_state *state)
{
	g_55e508 = NULL;
	if (function_147090() - g_55e504)
	{
		state->changed = true;
		g_55e4fc = true;
	}
}

// @retail 0x278eb0
void __cdecl function_278eb0(void *context)
{
	g_55e50c = context;
	g_55e500 = function_147090();
}

// @retail 0x278ed0
void __cdecl function_278ed0(s_278e80_state *state)
{
	g_55e50c = NULL;
	if (function_147090() - g_55e500)
	{
		state->changed = true;
		g_55e4fc = true;
	}
}

struct s_2797a0_group
{
	byte unknown00[0x40];
	long count;
};

struct s_2797a0_groups
{
	byte unknown00[8];
	s_2797a0_group **first;
	long first_count;
	long unknown10;
	s_2797a0_group **second;
	long second_count;
};

struct s_2797a0_iterator
{
	byte unknown00;
	bool second;
	byte unknown02[2];
	long group;
	long index;
	s_2797a0_groups *groups;
	long mode;
};

// @retail 0x2797a0
bool function_2797a0(s_2797a0_iterator *iterator)
{
	bool result = false;
	if (iterator->group == NONE)
	{
		iterator->group = 0;
		iterator->index = 0;
		if (iterator->mode == 1)
			iterator->second = true;
	}
	else
		iterator->index++;
	if (!iterator->second)
	{
		s_2797a0_groups *groups = iterator->groups;
		while (iterator->group < iterator->groups->first_count)
		{
			if (iterator->index < groups->first[iterator->group]->count)
			{
				result = true;
				break;
			}
			iterator->group++;
			iterator->index = 0;
		}
		if (!result)
		{
			iterator->second = true;
			iterator->group = 0;
			iterator->index = 0;
		}
	}
	if (iterator->second && (iterator->mode == 2 || iterator->mode == 1))
	{
		s_2797a0_groups *groups = iterator->groups;
		while (iterator->group < iterator->groups->second_count)
		{
			if (iterator->index < groups->second[iterator->group]->count)
			{
				result = true;
				break;
			}
			iterator->group++;
			iterator->index = 0;
		}
	}
	return result;
}
