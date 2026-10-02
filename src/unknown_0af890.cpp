// @flags /O2 /Gr
#include "cseries.h"
#include "unknown_1946f0.h"
#include "unknown_0af890.h"

// @retail 0x000af890
void __stdcall function_0af890(s_bitstream *stream, long size, s_message_0af890 *message)
{
	long i;

	function_1955d0(stream, message->nonce, 64);
	function_195720(stream, message->value08, 32);
	if (message->value0c == NONE)
	{
		stream_write_bit(stream, true);
	}
	else
	{
		stream_write_bit(stream, false);
		function_195720(stream, message->value0c, 32);
	}

	stream_write_bit(stream, message->has_a);
	if (message->has_a)
	{
		stream_write_bit(stream, message->a_flag);
		function_195720(stream, message->a_value, 32);
		stream_write_checked(stream, message->a_count, 5);
	}

	stream_write_bit(stream, message->has_b);
	if (message->has_b)
	{
		stream_write_checked(stream, message->b_value, 4);
	}

	stream_write_bit(stream, message->has_c);
	if (message->has_c)
	{
		stream_write_checked(stream, message->c_value, 2);
	}

	stream_write_bit(stream, message->has_d);
	if (message->has_d)
	{
		stream_write_bit(stream, message->has_d2);
		if (message->has_d2)
			function_1955d0(stream, message->d_data, 64);
	}

	stream_write_bit(stream, message->has_e);
	if (message->has_e)
	{
		stream_write_checked(stream, message->e_value0 - 1, 4);
		stream_write_checked(stream, message->e_value1 - 1, 4);
	}

	stream_write_bit(stream, message->has_f);
	if (message->has_f)
		stream_write_checked(stream, message->f_value, 2);

	stream_write_bit(stream, message->has_g);
	if (message->has_g)
		stream_write_bit(stream, message->g_flag);

	stream_write_bit(stream, message->has_h);
	if (message->has_h)
		stream_write_checked(stream, message->h_value + 1, 5);

	stream_write_bit(stream, message->has_i);
	if (message->has_i)
		stream_write_bit(stream, message->i_flag);

	stream_write_bit(stream, message->has_j);
	if (message->has_j)
	{
		stream_write_checked(stream, message->j_type, 3);
		if (message->j_type == 1)
		{
			stream_write_checked(stream, message->j_value0, 4);
			stream_write_checked(stream, message->j_value1, 4);
			stream_write_checked(stream, message->j_value2, 6);
			stream_write_checked(stream, message->j_value3, 10);
		}
		else if (message->j_type == 2)
		{
			stream_write_checked(stream, message->j_value4, 10);
		}
		else if (message->j_type == 3)
		{
			stream_write_checked(stream, message->j_value5, 10);
		}
	}

	stream_write_bit(stream, message->has_k);
	if (message->has_k)
		stream_write_checked(stream, message->k_value, 5);

	stream_write_bit(stream, message->has_m);
	if (message->has_m)
	{
		stream_write_bit(stream, message->has_m2);
		if (message->has_m2)
			function_063690(message->m_data, stream);
	}

	stream_write_bit(stream, message->has_l);
	if (message->has_l)
	{
		stream_write_bit(stream, message->has_l2);
		if (message->has_l2)
			function_1955d0(stream, message->l_data, 64);
	}

	stream_write_bit(stream, message->has_n);
	if (message->has_n)
	{
		function_195720(stream, message->n_value0, 32);
		function_195720(stream, message->n_value1, 32);
		i = 0;
		do
		{
			byte character = message->name[i];
			function_195720(stream, character, 8);
			if (!character)
				break;
			i++;
		} while (i < 128);
	}

	stream_write_bit(stream, message->has_o);
	if (message->has_o)
		function_1955d0(stream, message->o_data, 64);

	stream_write_bit(stream, message->has_p);
	if (message->has_p)
		function_195720(stream, message->p_value, 32);

	stream_write_bit(stream, message->has_q);
	if (message->has_q)
		stream_write_checked(stream, message->q_value, 2);

	stream_write_bit(stream, message->has_r);
	if (message->has_r)
		function_07cc50(stream, message->r_data);

	stream_write_bit(stream, message->has_s);
	if (message->has_s)
	{
		for (i = 0; i < 32; i++)
		{
			word value = message->words[i];
			function_195720(stream, value, 16);
			if (!value)
				break;
		}
	}

	stream_write_bit(stream, message->has_t);
	if (message->has_t)
	{
		stream_write_bit(stream, message->has_t2);
		if (message->has_t2)
		{
			stream_write_checked(stream, message->t_mask, 16);
			for (i = 0; i < 16; i++)
			{
				if (message->t_mask & (1 << i))
					function_1955d0(stream, message->t_data[i], 48);
			}

			for (i = 0; i < 16; i++)
			{
				s_message_0af890_entry *entry = &message->entries[i];
				stream_write_bit(stream, entry->present);
				if (entry->present)
				{
					stream_write_bit(stream, entry->flag);
					if (!entry->flag)
					{
						function_1955d0(stream, entry->address, 48);
						stream_write_checked(stream, entry->count0, 2);
						stream_write_checked(stream, entry->count1, 2);
					}
					function_1955d0(stream, entry->key, 96);
					function_07c5a0(stream, entry->data);
				}
			}
		}
	}

	stream_write_bit(stream, message->has_u);
	if (message->has_u)
		stream_write_bit(stream, message->has_u2);

	stream_write_bit(stream, message->has_v);
	if (message->has_v)
		stream_write_checked(stream, message->v_value + 1, 3);

	stream_write_bit(stream, message->has_w);
	if (message->has_w)
	{
		stream_write_bit(stream, message->has_w2);
		stream_write_checked(stream, message->w_value, 12);
		stream_write_checked(stream, message->w_type, 2);
		if (message->w_type == 1 || message->w_type == 2)
			stream_write_checked(stream, message->w_value2, 12);
		if (message->w_type == 1)
			function_1955d0(stream, message->w_data, 96);
	}

	stream_write_bit(stream, message->has_x);
	if (message->has_x)
	{
		stream_write_bit(stream, message->has_x2);
		if (message->has_x2)
			function_0b2330(message->x_data, stream);
	}

	stream_write_bit(stream, message->has_y);
	if (message->has_y)
		stream_write_checked(stream, message->y_value + 1, 5);
}
