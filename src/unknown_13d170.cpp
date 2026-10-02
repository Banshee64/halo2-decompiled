#include "cseries.h"
#include <string.h>
#include "globals.h"

// @flags /O2 /Gr

/* a resource manager: a named, doubly linked list of entries kept in a data
   array that follows the manager (0x18 bytes per entry; the links are the
   handles at +0xc and +0x10) */
typedef void (__stdcall *t_delete_proc)(long handle);

struct s_resource_entry
{
	byte unknown00[0xc];
	long next;
	long previous;
	byte unknown14[4];
};

struct s_resource_manager
{
	char name[0x20];
	t_delete_proc delete_proc;
	long unknown24;
	long unknown28;
	long unknown2c;
	long unknown30;
	long unknown34;
	long unknown38;
	long first;
	long last;
	long limits[8];
	s_data_array *data;
	dword signature;
	long unknown6c;
};

// @retail 0x13d170
void function_13d170(s_resource_manager *manager, const char *name, long a3, long a4, long maximum_count, t_delete_proc delete_proc, long a7, long a8, long a9)
{
	s_data_array *data = (s_data_array *)(manager + 1);

	data_initialize(data, name, maximum_count, sizeof(s_resource_entry), 0, g_46875c);
	data->valid = 1;
	data_delete_all(data);

	memset(manager, 0, sizeof(s_resource_manager));
	strncpy(manager->name, name, 0x20);
	manager->delete_proc = delete_proc;
	manager->unknown28 = a8;
	manager->unknown34 = a4;
	manager->unknown24 = a7;
	manager->data = data;
	manager->unknown38 = 1;
	manager->unknown30 = a3;
	manager->signature = 0x77656565;
	manager->name[0x1f] = 0;
	manager->unknown2c = 0;
	manager->unknown6c = a9;
	manager->first = NONE;
	manager->last = NONE;
	manager->limits[0] = 0x7fffffff;
	manager->limits[1] = 0x7fffffff;
	manager->limits[2] = 0x7fffffff;
	manager->limits[3] = 0x7fffffff;
	manager->limits[4] = 0x7fffffff;
	manager->limits[5] = 0x7fffffff;
	manager->limits[6] = 0x7fffffff;
	manager->limits[7] = 0x7fffffff;
}

// @retail 0x13d830
void function_13d830(s_resource_manager *manager, long handle)
{
	s_data_array *data = manager->data;
	s_resource_entry *entry = &((s_resource_entry *)data->data)[handle & 0xffff];

	if (manager->delete_proc)
		manager->delete_proc(handle);

	if (entry->previous != NONE)
		((s_resource_entry *)manager->data->data)[entry->previous & 0xffff].next = entry->next;
	else
		manager->first = entry->next;

	if (entry->next != NONE)
	{
		((s_resource_entry *)manager->data->data)[entry->next & 0xffff].previous = entry->previous;
		datum_delete(manager->data, handle);
	}
	else
	{
		manager->last = entry->previous;
		datum_delete(manager->data, handle);
	}
}
