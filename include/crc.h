/* CRC.H: CRC-32 checksums */

#ifndef CRC_H
#define CRC_H

void crc_new(dword *crc_reference);
void crc_checksum_buffer(dword *crc_reference, void const *buffer, long buffer_size);

#endif
