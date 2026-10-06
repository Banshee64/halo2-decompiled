// @flags /O2 /Gr
/* UNKNOWN_08E090.CPP: opening and closing the network link and the
   observer's activity with the network (lane D). Nothing in retail calls
   these or holds their addresses. */

#include "unknown_11c920.h"
#include "unknown_075870.h"

class c_class_93590;

/* unknown_0820f0.cpp */
extern bool g_4d8ba0;

/* the link and the observer of the network */
c_class_93590 *g_510560;
s_network_observer *g_510570;

/* unknown_092870.cpp */
bool network_link_open(c_class_93590 *link);
void network_link_close(c_class_93590 *link);
void network_link_close_connections(c_class_93590 *link);

/* 0x8e090 and 0x8e0b0 keep the standard convention (ret 4, the argument
   unused): no data or code in retail holds their addresses and nothing calls
   them. Their bodies match byte for byte with the standard marker; without
   it LTCG drops the unused argument (plain ret). */
// @retail 0x8e090 standard
void __stdcall function_08e090(long unused)
{
	if (g_4d8ba0)
	{
		c_class_93590 *link = g_510560;
		if (link)
			network_link_open(link);
	}
}

// @retail 0x8e0b0 standard
void __stdcall function_08e0b0(long unused)
{
	if (g_4d8ba0)
	{
		if (g_510560)
		{
			network_link_close_connections(g_510560);
			network_link_close(g_510560);
		}
		if (g_510570)
			g_510570->flag4e00 = false;
	}
}

#include <xtl.h>
#include "globals.h"
#include "unknown_059ad0.h"
#include "unknown_067e10.h"

struct s_network_message_gateway;
struct c_entry_table;
s_network_message_gateway *g_510568;
c_class_58d20 *g_510574;
void function_5a090(c_class_58d20 *session);
void network_observer_update(s_network_observer *observer);
void entry_table_flush_updates(c_entry_table *table);
void network_link_update_connections(c_class_93590 *link);
void network_message_gateway_send_pending_messages(s_network_message_gateway *gateway);

// @retail 0x8dfc0
void function_8dfc0(void)
{
 if (g_4d8ba0)
 {
  g_510548 = true;
  g_51054c = GetTickCount();
  for (long offset = 0; offset < 3 * 0x78b8; offset += 0x78b8)
   function_5a090((c_class_58d20 *)((byte *)g_510574 + offset));
  network_observer_update(g_510570);
  if (!g_4cf772)
  {
   c_class_6a600 *world = (c_class_6a600 *)g_4cf77c;
   long state = world->state;
   if (state && g_4e6948 && g_4e6948->flag1120 && (state == 4 || state == 5) && state != 3 && state != 5)
    entry_table_flush_updates((c_entry_table *)&world->distribution->field_2098);
  }
  network_link_update_connections(g_510560);
  network_message_gateway_send_pending_messages(g_510568);
  g_510548 = false;
  g_51054c = GetTickCount();
 }
}
