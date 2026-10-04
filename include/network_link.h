/* NETWORK_LINK.H: the network link (src/network_link.cpp), as the message
   gateway (src/unknown_07b330.cpp) sends through it */

#ifndef NETWORK_LINK_H
#define NETWORK_LINK_H

#include "cseries.h"
#include "transport_address.h"
#include "bitstream.h"

class c_network_link;

void network_link_send_out_of_band(c_network_link *link, s_bitstream const *stream, transport_address const *address, long *size_out);

#endif
