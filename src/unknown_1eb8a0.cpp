#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_123b30.h"
#include "kill_volumes.h"
#include <math.h>
#include <string.h>

// @flags /O2 /Ob1 /arch:SSE /Gr

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define PIN(x, lo, hi) ((x) < (lo) ? (lo) : (x) > (hi) ? (hi) : (x))

/* a timed follow/ease state: counts ticks (counter) towards a duration */
struct s_ease_state
{
	long ticks_left;
	long counter;
	long duration;
	bool fast;
	bool active;
	bool limited;
	byte pad0f;
	point3f start;
	vector3f impulse;
	real alignment;
	real distance;
	point3f target;
	vector3f direction;
	real limit;
	bool settled;
	byte pad4d[3];
	real settle_distance;
	real settle_minimum;
};

struct s_ease_output
{
	dword flags;
	byte unknown04[8];
	vector3f position;
};

real function_30bf0(vector3f *v);

// @retail 0x1eb8a0
long function_1eb8a0(
	bool update,
	vector3f *a,
	point3f *b,
	s_ease_state *s,
	s_ease_output *out,
	real dt,
	vector3f *d,
	vector3f *q,
	point3f *pos)
{
	s_game_time_globals *globals = g_510c54;
	bool fail = false;

	if (update)
	{
		if (!s->active)
		{
			if (dt > 0.001f)
			{
				real step = dt;
				if (s->limit != 0.f && step > s->limit)
					step = s->limit;

				real k = s->fast ? 12.f : 8.f;
				real t = step * 0.375f;
				real e = PIN(t, 0.001f, globals->rate * k);
				real third = e * 0.33333334f;
				real up = e * 4.f;
				real down = (e - third) * 3.f;
				real rem = step - up * 0.5f - down * 0.5f;
				if (rem < 0.f)
					rem = 0.f;
				real n = 6.f;
				if (e > 0.001f)
					n = rem / e + 6.f;

				s->alignment = d->k * a->k + d->j * a->j + a->i * d->i;
				real v = s->alignment * 1.5f;
				vector3f delta;
				delta.i = b->x - pos->x;
				delta.j = b->y - pos->y;
				delta.k = b->z - pos->z;
				v = PIN(v, 0.f, 2.5f);
				v = v * 0.4f;

				real len = (real)sqrt(delta.i * delta.i + delta.k * delta.k + delta.j * delta.j);
				if (len > 0.001f)
				{
					real scale = dt / len;
					delta.i = delta.i * scale;
					delta.j = delta.j * scale;
					delta.k = delta.k * scale;
				}

				real f = globals->rate * n * v;
				s->impulse.i = a->i * f;
				s->impulse.j = a->j * f;
				s->impulse.k = a->k * f;
				delta.i = delta.i + s->impulse.i;
				delta.j = delta.j + s->impulse.j;
				delta.k = delta.k + s->impulse.k;

				real len2 = (real)sqrt(delta.i * delta.i + delta.k * delta.k + delta.j * delta.j);
				real len3;
				if (s->limit != 0.f && len2 > s->limit)
					len3 = s->limit;
				else
					len3 = len2;

				real k2 = s->fast ? 12.f : 8.f;
				real t2 = len3 * 0.375f;
				real e2 = PIN(t2, 0.001f, globals->rate * k2);
				real third2 = e2 * 0.33333334f;
				real up2 = e2 * 4.f;
				real down2 = (e2 - third2) * 3.f;
				real rem2 = len3 - up2 * 0.5f - down2 * 0.5f;
				if (rem2 < 0.f)
					rem2 = 0.f;
				real n2 = 6.f;
				if (e2 > 0.001f)
					n2 = rem2 / e2 + 6.f;

				s->limited = s->limit != 0.f && len2 > s->limit;
				s->start = *pos;
				s->distance = s->limited ? s->limit : len2;
				s->active = true;
				s->duration = (long)((s->fast ? 7 : 1) + n2);
				if (len2 > 0.001f)
				{
					real r = s->distance / len2;
					s->target.x = delta.i * r + pos->x;
					s->target.y = delta.j * r + pos->y;
					s->target.z = delta.k * r + pos->z;
				}
				else
				{
					s->target = *pos;
				}
				s->direction.i = s->target.x - pos->x;
				s->direction.j = s->target.y - pos->y;
				s->direction.k = s->target.z - pos->z;
				if (function_30bf0(&s->direction) == 0.f)
					s->direction = *d;
			}
		}

		if (s->active)
		{
			if (!s->limited)
			{
				s->target.x = s->direction.i * dt + pos->x;
				s->target.y = s->direction.j * dt + pos->y;
				s->target.z = s->direction.k * dt + pos->z;
				real w = PIN(s->alignment, 0.f, 2.5f);
				w = w * 0.2f;
				s->target.x = s->direction.i * w + s->target.x;
				s->target.y = s->direction.j * w + s->target.y;
				s->target.z = s->direction.k * w + s->target.z;
			}
			s->direction.i = s->target.x - pos->x;
			s->direction.j = s->target.y - pos->y;
			s->direction.k = s->target.z - pos->z;
			if (function_30bf0(&s->direction) == 0.f)
				s->direction = *d;
		}

		if (0.8660254f > s->direction.k * d->k + s->direction.j * d->j + d->i * s->direction.i)
			s->direction = *d;
	}

	out->position = *q;

	if (s->active)
	{
		real k3 = s->fast ? 12.f : 8.f;
		real t3 = s->distance * 0.375f;
		real e3 = PIN(t3, 0.001f, globals->rate * k3);
		long counter = s->counter;
		real cnt = (real)counter;
		real w = s->direction.k * q->k + s->direction.j * q->j + q->i * s->direction.i;
		if (counter == 0)
			q = g_4687a4;
		w = w * globals->rate;
		vector3f qr = *q;
		real third3 = e3 * 0.33333334f;
		real lo = (w - third3) * 3.f * 0.5f;
		real hi = (e3 - third3) * 3.f * 0.5f;
		if (lo < 0.f)
			lo = 0.f;
		vector3f dl;
		dl.i = s->target.x - pos->x;
		qr.i = qr.i * globals->rate;
		dl.j = s->target.y - pos->y;
		qr.j = qr.j * globals->rate;
		dl.k = s->target.z - pos->z;
		qr.k = qr.k * globals->rate;
		real dist = (real)sqrt(dl.k * dl.k + dl.j * dl.j + dl.i * dl.i);

		if (e3 > 0.001f)
		{
			if (!s->settled && !(lo > dist) && 4.f < (real)(s->duration - counter))
			{
				if (5.f > cnt)
				{
					real f = MIN(third3, MAX(0.f, e3 - MAX(0.f, w)));
					out->position.i = s->direction.i * f + qr.i;
					out->position.j = s->direction.j * f + qr.j;
					out->position.k = s->direction.k * f + qr.k;
					real ticks = (real)globals->field_2_3;
					out->position.i = out->position.i * ticks;
					out->position.j = out->position.j * ticks;
					out->position.k = out->position.k * ticks;
					s->ticks_left = (long)((dist - (((3.f - cnt) - 0.5f) * e3 * 0.5f + hi)) / e3 + (3.f - cnt) + 3.f + 0.5f);
				}
				else
				{
					out->position.i = qr.i;
					out->position.j = qr.j;
					out->position.k = qr.k;
					real ticks = (real)globals->field_2_3;
					out->position.i = ticks * out->position.i;
					out->position.j = out->position.j * ticks;
					out->position.k = out->position.k * ticks;
					s->ticks_left = (long)((dist - hi) / e3 + 3.f - 0.5f);
				}
				goto done;
			}

			{
				real v = PIN((double)s->alignment * 1.5, 0.75, 3.5);
				real step = (real)(globals->rate * v);
				vector3f nq = qr;
				real len = (real)sqrt(qr.k * qr.k + qr.j * qr.j + qr.i * qr.i);
				real nlen = function_30bf0(&nq);
				if (!s->settled)
				{
					s->settled = true;
					s->settle_distance = MAX(step, len - step);
					s->settle_minimum = MAX(0.001f, dist);
				}
				real align = d->j * nq.j + d->k * nq.k + d->i * nq.i;
				if (align > len * 0.08715574f && nlen != 0.f)
				{
					real m = s->settle_distance * 0.25f;
					if (len > 0.001f)
					{
						real len4 = MIN(m, len);
						real ratio = s->settle_minimum / hi - 0.5f;
						real back = 0.f - len4;
						out->position.j = nq.j * back + qr.j;
						out->position.i = nq.i * back + qr.i;
						out->position.k = nq.k * back + qr.k;
						real ticks = (real)globals->field_2_3;
						out->position.i = out->position.i * ticks;
						out->position.j = out->position.j * ticks;
						out->position.k = out->position.k * ticks;
						s->ticks_left = (long)ratio;
						goto done;
					}
				}
				else
				{
					real m = (s->settle_distance + step) * 0.33333334f;
					real m2 = MIN(m, len);
					real back = 0.f - m2;
					out->position.i = back * nq.i + qr.i;
					out->position.j = nq.j * back + qr.j;
					out->position.k = nq.k * back + qr.k;
					if (!(step > len - m + 0.001f))
						goto done;
				}
			}
		}
		else
		{
			s->settled = true;
		}
		fail = true;
		s->ticks_left = 0;
	}

done:
	long new_counter = s->counter + 1;
	s->counter = new_counter;
	if (!fail && s->ticks_left > 0 && new_counter < s->duration + 6)
		out->flags &= ~4;
	else
		out->flags |= 4;
	return new_counter;
}

struct s_unknown_object
{
	byte unknown00[0xbc];
	bool flag_bc;
	byte unknownbd[0x1b];
	real value_d8;
};

struct s_unknown_output
{
	long unknown00;
	long mode;
};

// @retail 0x1ec3f0
void function_1ec3f0(s_unknown_object *object, s_unknown_output *output)
{
	if (object->flag_bc && object->value_d8 >= 0.1f)
		output->mode = 4;
}

s_kill_volume_globals *g_51e9c8;

// @retail 0x1ec420
void function_1ec420(void)
{
	s_kill_volume_globals *globals = (s_kill_volume_globals *)function_123d40("unknown", "unknown", sizeof(s_kill_volume_globals));
	g_51e9c8 = globals;
	globals->enabled = false;
}

// @retail 0x1ec470
void function_1ec470(void)
{
	g_51e9c8 = 0;
}

// @retail 0x1ec480
void function_1ec480(void)
{
	s_kill_volume_globals *globals = g_51e9c8;
	memset(globals->bits, 0, sizeof(globals->bits));
	globals->enabled = true;
}

// @retail 0x1ec4b0
void function_1ec4b0(void)
{
	g_51e9c8->enabled = false;
}

struct s_unknown_entry
{
	byte unknown00[0x40];
	short bit_index;
	byte unknown42[2];
};

// @retail 0x1ec4c0
void function_1ec4c0(long index)
{
	if (index != NONE)
	{
		s_unknown_entry *entry = g_4e0350->entries + index;
		if (entry->bit_index != NONE)
		{
			g_51e9c8->bits[entry->bit_index >> 5] &= ~(1 << (entry->bit_index & 0x1f));
		}
	}
}
