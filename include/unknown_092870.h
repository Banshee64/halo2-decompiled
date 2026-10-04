/* UNKNOWN_092870.H: the network link (src/unknown_092870.cpp), as the message
   gateway (src/unknown_07b330.cpp) sends through it */

#ifndef NETWORK_LINK_H
#define NETWORK_LINK_H

#include "unknown_11c920.h"
#include "unknown_07aec0.h"
#include "bitstream.h"

class c_class_93590;

void network_link_send_out_of_band(c_class_93590 *link, s_bitstream const *stream, s_type_99af70 const *address, long *size_out);

#endif
