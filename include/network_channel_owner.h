/* NETWORK_CHANNEL_OWNER.H: what owns a set of the observer's channels (a
   session, vtable 0x450df8): the observer asks it about its channels and
   tells it when one of them connects or closes */

#ifndef NETWORK_CHANNEL_OWNER_H
#define NETWORK_CHANNEL_OWNER_H

#include "cseries.h"

class c_network_channel_owner
{
public:
	virtual bool channel_is_host_or_local(long channel_index) { return false; }
	virtual bool channel_is_trusted(long channel_index) { return false; }
	virtual bool channel_may_send(long channel_index, bool force) { return false; }
	virtual void channel_connection_changed(long channel_index, long remote_index, bool connected) {}
	virtual void channel_closed(long channel_index) {}
	virtual long find_member_by_channel(long channel_index) { return NONE; }
};

#endif