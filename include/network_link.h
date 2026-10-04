/* NETWORK_LINK.H: the network link (src/field_4_5.cpp), as the message
   gateway (src/unknown_07b330.cpp) sends through it */

#ifndef NETWORK_LINK_H
#define NETWORK_LINK_H

#include "cseries.h"
#include "transport_address.h"
#include "bitstream.h"

class c_class_93590;

void network_link_send_out_of_band(c_class_93590 *link, s_bitstream const *stream, s_type_99af70 const *address, long *size_out);

#endif
