/* CRC.H: CRC-32 checksums */

#ifndef CRC_H
#define CRC_H

void function_x86aaf2(dword *crc_reference);
void function_163ba0(dword *crc_reference, void const *buffer, long buffer_size);

#endif
