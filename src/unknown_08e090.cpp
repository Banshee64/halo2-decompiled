// @flags /O2 /Gr
/* UNKNOWN_08E090.CPP: opening and closing the network link and the
   observer's activity with the network (lane D). Nothing in retail calls
   these or holds their addresses. */

#include "cseries.h"
#include "network_observer.h"

class c_class_93590;

/* network_connection.cpp */
extern bool g_4d8ba0;

/* the link and the observer of the network */
c_class_93590 *g_510560;
s_network_observer *g_510570;

/* field_4_5.cpp */
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