#include "cseries.h"
#include "real_math.h"

// @flags /O2 /arch:SSE /Gr

/* UNKNOWN_13B390.CPP: tag function definitions. A function is a tag data
   block of bytes: a type, flags (bit 0: ranged, the high nibble: the output
   kind), the transition or periodic function types of the low and high
   ranges, the output bounds, then the type's parameters, once per range. */

struct s_tag_data
{
	long size;
	byte *address;
};

enum
{
	_function_identity = 0,
	_function_constant,
	_function_transition,
	_function_periodic,
	_function_linear,
	_function_linear_key,
	_function_multi_linear_key,
	_function_spline,
	_function_multi_spline,
	_function_exponent,
	_function_spline2,
	k_number_of_function_types
};

struct s_function_header
{
	byte type;
	byte flags;
	byte function_type[2];
	byte unknown04[0x14 - 0x04];
};

/* a spline's parameters per range: its control points and the cubic's
   coefficients */
struct s_function_spline
{
	s_function_header header;
	real_point2d points[4];
	real coefficients[4];
};

/* the number of parameters of each function type, per range */
byte const g_44178c[k_number_of_function_types] = { 0, 1, 2, 4, 6, 20, 32, 12, 4, 3, 12 };

real function_17c900(short function_type, real input);
real function_17ca10(real x, short curve);

/* sqrt(a) / sqrt(b), approximately: the square root of a from its bits,
   times rsqrtss of b */
static __forceinline real square_root_ratio_approximate(real a, real b)
{
	long bits = (*(long *)&a >> 1) + 0x1fc00000;
	real square_root = *(real *)&bits;
	real reciprocal;

	__asm
	{
		rsqrtss xmm0, b
		movss reciprocal, xmm0
		fld reciprocal
		fmul square_root
	}
}

struct real_vector2d
{
	real i, j;
};

static inline void vector_from_points2d(real_point2d const *p0, real_point2d const *p1, real_vector2d *out)
{
	out->i = p1->x - p0->x;
	out->j = p1->y - p0->y;
}

static inline real magnitude_squared2d(real_vector2d const *v)
{
	return v->i * v->i + v->j * v->j;
}

static inline real interpolate_linear(real a, real b, real t)
{
	return (1.0f - t) * a + b * t;
}

// @retail 0x13bec0
real_point2d *function_13bec0(s_tag_data const *function, byte range_index, byte point_index)
{
	s_function_header *header = (s_function_header *)function->address;
	if (header->type >= _function_linear && (header->type <= _function_multi_spline || header->type == _function_spline2))
	{
		return (real_point2d *)((real *)(header + 1) + g_44178c[header->type] * range_index + point_index * 2);
	}
	return NULL;
}

// @retail 0x13b390
real function_13b390(void const *function, real input, real range)
{
	s_tag_data const *tag_data = (s_tag_data const *)function;
	real result = 0.0f;

	if (tag_data->address && tag_data->size > 0)
	{
		s_function_header const *header = (s_function_header const *)tag_data->address;
		real const *parameters = (real const *)(header + 1);

		switch (header->type)
		{
		case _function_identity:
			result = input;
			break;
		case _function_constant:
			if (header->flags & 1)
			{
				result = range;
			}
			else
			{
				result = 0.0f;
			}
			break;
		case _function_transition:
		{
			real x = function_17ca10(input, header->function_type[0]);
			result = (parameters[1] - parameters[0]) * x + parameters[0];
			if (header->flags & 1)
			{
				parameters += 2;
				x = function_17ca10(input, header->function_type[1]);
				real high = (parameters[1] - parameters[0]) * x + parameters[0];
				result = interpolate_linear(result, high, range);
			}
			break;
		}
		case _function_exponent:
		{
			real exponent = parameters[2];
			if (fabs(exponent) < 0.0001f || (0.0f > exponent && fabs(input) < 0.0001f))
			{
				result = 1.0f;
			}
			else
			{
				result = (real)((parameters[1] - parameters[0]) * pow(input, exponent) + parameters[0]);
			}
			if (header->flags & 1)
			{
				parameters += 3;
				real high;
				exponent = parameters[2];
				if (fabs(exponent) < 0.0001f || (0.0f > exponent && fabs(input) < 0.0001f))
				{
					high = 1.0f;
				}
				else
				{
					high = (real)((parameters[1] - parameters[0]) * pow(input, exponent) + parameters[0]);
				}
				result = interpolate_linear(result, high, range);
			}
			break;
		}
		case _function_periodic:
		{
			real x = function_17c900(header->function_type[0], parameters[0] * input + parameters[1]);
			result = (parameters[3] - parameters[2]) * x + parameters[2];
			if (header->flags & 1)
			{
				parameters += 4;
				x = function_17c900(header->function_type[1], parameters[0] * input + parameters[1]);
				real high = (parameters[3] - parameters[2]) * x + parameters[2];
				result = interpolate_linear(result, high, range);
			}
			break;
		}
		case _function_linear:
			result = parameters[4] * input + parameters[5];
			if (header->flags & 1)
			{
				parameters += 6;
				result = interpolate_linear(result, parameters[4] * input + parameters[5], range);
			}
			break;
		case _function_linear_key:
		{
			real value;
			value = (input - parameters[9]) * parameters[13];
			real t0 = 0.0f > value ? 0.0f : (value > 1.0f ? 1.0f : value);
			value = (input - parameters[10]) * parameters[14];
			real t1 = 0.0f > value ? 0.0f : (value > 1.0f ? 1.0f : value);
			value = (input - parameters[11]) * parameters[15];
			real t2 = 0.0f > value ? 0.0f : (value > 1.0f ? 1.0f : value);
			result = parameters[19] * t2 + parameters[18] * t1 + parameters[17] * t0 + parameters[16];
			if (header->flags & 1)
			{
				parameters += 20;
				value = (input - parameters[9]) * parameters[13];
				t0 = 0.0f > value ? 0.0f : (value > 1.0f ? 1.0f : value);
				value = (input - parameters[10]) * parameters[14];
				t1 = 0.0f > value ? 0.0f : (value > 1.0f ? 1.0f : value);
				value = (input - parameters[11]) * parameters[15];
				t2 = 0.0f > value ? 0.0f : (value > 1.0f ? 1.0f : value);
				real high = parameters[19] * t2 + parameters[18] * t1 + parameters[17] * t0 + parameters[16];
				result = interpolate_linear(result, high, range);
			}
			break;
		}
		case _function_spline:
		case _function_spline2:
		{
			real t = input;
			if (header->type == _function_spline2)
			{
				real_point2d const *p0 = function_13bec0(tag_data, 0, 0);
				real_point2d const *p1 = function_13bec0(tag_data, 0, 1);
				real_point2d const *p2 = function_13bec0(tag_data, 0, 2);
				real_point2d const *p3 = function_13bec0(tag_data, 0, 3);
				real_vector2d vector;
				vector_from_points2d(p3, p2, &vector);
				real end_length_squared = magnitude_squared2d(&vector);
				if (end_length_squared > 0.0001f)
				{
					vector_from_points2d(p1, p0, &vector);
					real start_length_squared = magnitude_squared2d(&vector);
					if (fabs(start_length_squared) < 0.0001f)
					{
						t = 1.0f;
					}
					else if (!(fabs(start_length_squared - end_length_squared) < 0.0001f))
					{
						real ratio = square_root_ratio_approximate(start_length_squared, end_length_squared);
						real value = (real)pow(input, ratio);
						t = 0.0f > value ? 0.0f : (value > 1.0f ? 1.0f : value);
					}
				}
			}
			real const *coefficients = ((s_function_spline const *)header)->coefficients;
			real t2 = t * t;
			real t3 = t2 * t;
			result = coefficients[1] * t2 + coefficients[2] * t + coefficients[0] * t3 + coefficients[3];
			if (header->flags & 1)
			{
				coefficients += 12;
				if (header->type == _function_spline2)
				{
					real_point2d const *p0 = function_13bec0(tag_data, 1, 0);
					real_point2d const *p1 = function_13bec0(tag_data, 1, 1);
					real_point2d const *p2 = function_13bec0(tag_data, 1, 2);
					real_point2d const *p3 = function_13bec0(tag_data, 1, 3);
					real_vector2d vector;
					vector_from_points2d(p3, p2, &vector);
					real end_length_squared = magnitude_squared2d(&vector);
					if (end_length_squared > 0.0001f)
					{
						vector_from_points2d(p1, p0, &vector);
						real start_length_squared = magnitude_squared2d(&vector);
						if (fabs(start_length_squared) < 0.0001f)
						{
							t = 1.0f;
						}
						else if (!(fabs(start_length_squared - end_length_squared) < 0.0001f))
						{
							real ratio = square_root_ratio_approximate(start_length_squared, end_length_squared);
							real value = (real)pow(input, ratio);
							t = 0.0f > value ? 0.0f : (value > 1.0f ? 1.0f : value);
						}
					}
					t2 = t * t;
					t3 = t2 * t;
				}
				real high = coefficients[1] * t2 + coefficients[2] * t + coefficients[0] * t3 + coefficients[3];
				result = interpolate_linear(result, high, range);
			}
			break;
		}
		default:
			return result;
		}

		if (0.0f > result)
		{
			return 0.0f;
		}
		if (result > 1.0f)
		{
			return 1.0f;
		}
	}

	return result;
}
