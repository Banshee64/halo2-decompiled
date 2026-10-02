/* UNKNOWN_0AF890.H: the large network message encoder at 0xaf890 and the
   session sub-codecs it calls */

#ifndef UNKNOWN_0AF890_H
#define UNKNOWN_0AF890_H

#include "cseries.h"
#include "bitstream.h"
#include "unknown_1946f0.h"

/* writes one bit; the codecs inline it */
inline void stream_write_bit(s_bitstream *stream, bool value)
{
	if ((stream->size_in_bytes << 3) - stream->bit_position >= 1 && value)
		stream->data[stream->bit_position / 8] |= (byte)(1 << (stream->bit_position % 8));
	stream->bit_position++;
}

/* the checked write the codecs inline (1947e0) */
inline void stream_write_checked(s_bitstream *stream, dword value, long bits)
{
	if (bits < 32 && value >= (dword)(1 << bits))
	{
		char message[256];
		message[0] = 0;
		csprintf_256(message, "%u exceeds max value of %u", value, 1 << bits);
	}
	function_195720(stream, value, bits);
}

struct s_message_0af890_entry
{
	bool present;
	bool flag;
	short count0;
	long count1;
	byte address[6];
	byte key[12];
	byte unknown1a[2];
	byte data[0xc8];
};

struct s_message_0af890
{
	byte nonce[8];
	dword value08;
	long value0c;
	bool has_a;
	bool a_flag;
	byte unknown12[2];
	dword a_count;
	dword a_value;
	bool has_b;
	byte unknown1d[3];
	dword b_value;
	bool has_c;
	byte unknown25[3];
	dword c_value;
	bool has_e;
	byte unknown2d[3];
	long e_value0;
	long e_value1;
	bool has_d;
	bool has_d2;
	byte d_data[8];
	bool has_f;
	byte unknown43;
	dword f_value;
	bool has_g;
	bool g_flag;
	bool has_h;
	byte unknown4b;
	long h_value;
	bool has_i;
	bool i_flag;
	bool has_j;
	byte unknown53;
	dword j_type;
	dword j_value0;
	dword j_value1;
	dword j_value2;
	dword j_value3;
	dword j_value4;
	dword j_value5;
	bool has_k;
	byte unknown71[3];
	dword k_value;
	bool has_l;
	bool has_l2;
	byte unknown7a[6];
	byte l_data[8];
	bool has_m;
	bool has_m2;
	byte unknown8a[2];
	byte m_data[0x308];
	bool has_n;
	byte unknown395[3];
	dword n_value0;
	dword n_value1;
	byte name[128];
	bool has_o;
	byte unknown421[7];
	byte o_data[8];
	bool has_p;
	byte unknown431[3];
	dword p_value;
	bool has_q;
	byte unknown439[3];
	dword q_value;
	bool has_r;
	byte unknown441[3];
	byte r_data[0x130];
	bool has_s;
	byte unknown575;
	word words[32];
	bool has_t;
	bool has_t2;
	dword t_mask;
	byte t_data[16][6];
	byte unknown61c[8];
	s_message_0af890_entry entries[16];
	bool has_u;
	bool has_u2;
	bool has_v;
	byte unknown1467;
	short v_value;
	bool has_w;
	bool has_w2;
	dword w_value;
	dword w_type;
	dword w_value2;
	byte w_data[12];
	bool has_x;
	bool has_x2;
	byte unknown1486[2];
	byte x_data[0x44];
	bool has_y;
	byte unknown14cd[3];
	dword y_value;
};

/* the session sub-codecs the message calls */
void __stdcall function_063690(void *data, s_bitstream *stream);
void __stdcall function_07cc50(s_bitstream *stream, void *data);
void __stdcall function_07c5a0(s_bitstream *stream, void *data);
void __stdcall function_0b2330(void *data, s_bitstream *stream);

#endif
