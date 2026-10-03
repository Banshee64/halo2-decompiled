// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_11FC80.CPP: polygon clipping, vector helpers, the worker job queue
   and a few profile setters */

#include "cseries.h"
#include "real_math.h"
#include "job_queue.h"
#include <math.h>
#include <string.h>
#include <xtl.h>

#define k_real_epsilon 0.0001f

real function_30bf0(real_vector3d *v);

/* ---- globals ---- */

/* the worker job queue (a mutex-guarded free list and a used list of 150
   nodes of 0x40 bytes; the node is in job_queue.h) */
s_job_node g_4e0368[150];
s_job_node *g_4e28e8;
s_job_node *g_4e28ec;
HANDLE g_4e0354;
HANDLE g_4e0358;
long g_4e035c;
HANDLE g_4e0360;

s_thread_stack g_5020c8;

/* the profile names (64 bytes each) and the flags the setters touch */
struct s_profile_name
{
	wchar_t name[32];
};

s_profile_name g_51084c[8];
bool g_510819;
long g_510990;
long g_510994;

extern short const g_440b94[6][3] =
{
	{ 2, 1, 0 },
	{ 1, 2, 0 },
	{ 0, 2, 1 },
	{ 2, 0, 1 },
	{ 1, 0, 2 },
	{ 0, 1, 2 },
};

long function_120c30(void *);

// @retail 0x11fc80
long function_11fc80(
	real_plane3d const *plane,
	bool keep_inside,
	real tolerance,
	long point_count,
	real_point3d const *points,
	long maximum_count,
	real_point3d *out)
{
	long count = 0;

	if (point_count > 0)
	{
		real offset = keep_inside ? tolerance : 0.f - tolerance;
		real_point3d const *previous = &points[point_count - 1];
		real previous_distance = plane->i * previous->x + plane->j * previous->y + plane->k * previous->z - plane->d - offset;
		bool previous_inside = previous_distance >= 0.f;

		for (long i = 0; i < point_count; i++)
		{
			real_point3d const *current = &points[i];
			real distance = plane->i * current->x + plane->j * current->y + plane->k * current->z - plane->d - offset;
			bool inside = distance >= 0.f;

			if (inside != previous_inside)
			{
				if (count >= maximum_count)
					break;
				real_vector3d delta;
				delta.i = current->x - previous->x;
				delta.j = current->y - previous->y;
				delta.k = current->z - previous->z;
				real t = previous_distance / (previous_distance - distance);
				out[count].x = delta.i * t + previous->x;
				out[count].y = t * delta.j + previous->y;
				out[count].z = t * delta.k + previous->z;
				count++;
			}
			if (inside == keep_inside)
			{
				if (count >= maximum_count)
					break;
				out[count] = *current;
				count++;
			}
			previous = current;
			previous_distance = distance;
			previous_inside = inside;
		}
	}
	return count;
}

static inline real distance_squared(real_point3d const *a, real_point3d const *b)
{
	real dx = a->x - b->x;
	real dy = a->y - b->y;
	real dz = a->z - b->z;

	return dz * dz + dy * dy + dx * dx;
}

/* two-bone inverse kinematics: moves the middle node so that the chain from
   the root reaches the target (clamped to the chain's reach), rebuilds the
   root and middle axes and copies the target into the end node */
// @retail 0x120220
void function_120220(
	real_matrix4x3 *mid,
	real_matrix4x3 *root,
	real_matrix4x3 *target,
	real_matrix4x3 *end)
{
	real mid_root_squared = distance_squared(&mid->position, &root->position);
	real end_mid_squared = distance_squared(&end->position, &mid->position);
	real root_target_squared = distance_squared(&root->position, &target->position);

	if (mid_root_squared > k_real_epsilon && end_mid_squared > k_real_epsilon && root_target_squared > k_real_epsilon)
	{
		real mid_root = (real)sqrt(mid_root_squared);
		real end_mid = (real)sqrt(end_mid_squared);
		real root_target = (real)sqrt(root_target_squared);

		real_vector3d delta;
		delta.i = target->position.x - root->position.x;
		delta.j = target->position.y - root->position.y;
		delta.k = target->position.z - root->position.z;
		real_vector3d to_mid;
		to_mid.i = mid->position.x - root->position.x;
		to_mid.j = mid->position.y - root->position.y;
		to_mid.k = mid->position.z - root->position.z;

		real inverse = 1.f / root_target;
		real_vector3d u;
		u.i = delta.i * inverse;
		u.j = delta.j * inverse;
		u.k = delta.k * inverse;

		real_vector3d normal;
		normal.i = u.j * to_mid.k - u.k * to_mid.j;
		normal.j = u.k * to_mid.i - u.i * to_mid.k;
		normal.k = u.i * to_mid.j - u.j * to_mid.i;
		function_30bf0(&normal);

		real_vector3d e;
		e.i = normal.j * u.k - normal.k * u.j;
		e.j = normal.k * u.i - normal.i * u.k;
		e.k = normal.i * u.j - normal.j * u.i;

		real distance = root_target;
		real limit = (end_mid + mid_root) * 0.98f;
		if (distance > limit)
		{
			target->position.x = limit * u.i + root->position.x;
			target->position.y = limit * u.j + root->position.y;
			target->position.z = limit * u.k + root->position.z;
			distance = limit;
		}

		real along = (distance * distance + mid_root * mid_root - end_mid * end_mid) / (distance * 2.f);
		real remaining = distance - along;
		real height_squared = mid_root * mid_root - along * along;
		real height;
		if (height_squared >= k_real_epsilon)
			height = (real)sqrt(height_squared);
		else
			height = 0.f;

		real_vector3d lift;
		lift.i = e.i * height;
		lift.j = e.j * height;
		lift.k = e.k * height;

		root->forward.i = along * u.i + lift.i;
		root->forward.j = along * u.j + lift.j;
		root->forward.k = along * u.k + lift.k;
		function_30bf0(&root->forward);

		root->up.i = root->forward.j * root->left.k - root->forward.k * root->left.j;
		root->up.j = root->forward.k * root->left.i - root->forward.i * root->left.k;
		root->up.k = root->left.j * root->forward.i - root->forward.j * root->left.i;
		function_30bf0(&root->up);

		root->left.i = root->forward.k * root->up.j - root->up.k * root->forward.j;
		root->left.j = root->up.k * root->forward.i - root->forward.k * root->up.i;
		root->left.k = root->forward.j * root->up.i - root->up.j * root->forward.i;

		real_point3d new_mid;
		new_mid.x = mid_root * root->forward.i + root->position.x;
		new_mid.y = root->forward.j * mid_root + root->position.y;
		new_mid.z = root->forward.k * mid_root + root->position.z;

		mid->forward.i = remaining * u.i - lift.i;
		mid->forward.j = remaining * u.j - lift.j;
		mid->forward.k = remaining * u.k - lift.k;
		function_30bf0(&mid->forward);

		mid->up.i = mid->forward.j * mid->left.k - mid->forward.k * mid->left.j;
		mid->up.j = mid->forward.k * mid->left.i - mid->forward.i * mid->left.k;
		mid->up.k = mid->forward.i * mid->left.j - mid->left.i * mid->forward.j;
		function_30bf0(&mid->up);

		mid->left.i = mid->forward.k * mid->up.j - mid->up.k * mid->forward.j;
		mid->left.j = mid->up.k * mid->forward.i - mid->forward.k * mid->up.i;
		mid->left.k = mid->forward.j * mid->up.i - mid->up.j * mid->forward.i;

		mid->position = new_mid;
		*end = *target;
	}
}

// @retail 0x1201a0
real function_1201a0(real_vector3d *v, real_vector3d const *fallback)
{
	real m = (real)sqrt(v->i * v->i + v->j * v->j + v->k * v->k);
	if (!(fabs(m) < k_real_epsilon))
	{
		real inv = 1.f / m;
		v->i = v->i * inv;
		v->j = inv * v->j;
		v->k = inv * v->k;
		return m;
	}
	*v = *fallback;
	return 0.f;
}

// @retail 0x120790
void function_120790(real *out, real const *plane, real const *point, short axis, byte side)
{
	short const *entry = g_440b94[side + axis * 2];
	short a = entry[0];
	short b = entry[1];

	out[a] = point[0];
	out[b] = point[1];
	if (fabs(plane[axis]) < k_real_epsilon)
		out[axis] = 0.f;
	else
		out[axis] = (plane[3] - plane[a] * point[0] - plane[b] * point[1]) / plane[axis];
}

// @retail 0x1208b0
s_job_node *function_1208b0(void)
{
	s_job_node *node = NULL;

	do
	{
		WaitForSingleObject(g_4e0358, (DWORD)-1);
		if (g_4e28e8)
		{
			node = g_4e28e8;
			g_4e28e8 = node->next;
			node->next = NULL;
		}
		ReleaseMutex(g_4e0358);
		if (!node)
			SwitchToThread();
	}
	while (!node);
	return node;
}

// @retail 0x120a30
void function_120a30(s_job_node *node)
{
	WaitForSingleObject((HANDLE)g_4e035c, (DWORD)-1);
	if (g_4e28ec == node)
	{
		g_4e28ec = node->next;
		node->state = NONE;
	}
	else
	{
		s_job_node *previous = g_4e28ec;
		while (previous->next != node)
			previous = previous->next;
		previous->next = node->next;
		node->state = NONE;
	}
	ReleaseMutex((HANDLE)g_4e035c);
}

// @retail 0x120a90
void function_120a90(void)
{
	memset(g_4e0368, 0, sizeof(g_4e0368));
	for (long i = 0; i < 150; i++)
	{
		g_4e0368[i].next = &g_4e0368[i + 1];
		g_4e0368[i].state = NONE;
	}
	g_4e0368[149].next = NULL;
	g_4e28e8 = g_4e0368;
	g_4e28ec = NULL;
	g_4e0358 = CreateMutexA(NULL, FALSE, NULL);
	g_4e035c = (long)CreateMutexA(NULL, FALSE, NULL);
	g_4e0360 = CreateSemaphoreA(NULL, 0, 150, NULL);
	g_5020c8.unknown00 = 0;
	g_5020c8.unknown04 = 0x4fa0c8;
	g_5020c8.unknown08 = 0x8000;
	g_4e0354 = CreateThread(NULL, 0x4000, (LPTHREAD_START_ROUTINE)function_120c30, NULL, 0, NULL);
	SetThreadPriority(g_4e0354, 1);
}

// @retail 0x120bf0
long function_120bf0(void)
{
	long count = 0;

	WaitForSingleObject((HANDLE)g_4e035c, (DWORD)-1);
	for (s_job_node *node = g_4e28ec; node; node = node->next)
		count++;
	ReleaseMutex((HANDLE)g_4e035c);
	return count;
}

// @retail 0x120df0
void function_120df0(long index, wchar_t const *name)
{
	s_profile_name *profile = &g_51084c[index];

	wcsncpy(profile->name, name, 31);
	profile->name[31] = 0;
	g_510819 = true;
}

// @retail 0x120e40
void function_120e40(wchar_t const *name)
{
	wcsncpy(g_51084c[4].name, name, 31);
	g_51084c[4].name[31] = 0;
	g_510819 = true;
}

// @retail 0x121040
void function_121040(long value)
{
	if (value != NONE)
	{
		g_510990 = value;
		g_510819 = true;
	}
}

// @retail 0x121060
void function_121060(long value)
{
	if (value < 0)
		g_510994 = 0;
	else
	{
		g_510994 = 3;
		if (!(value > 3))
			g_510994 = value;
	}
	g_510819 = true;
}
