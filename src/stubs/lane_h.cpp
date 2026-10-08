// Stubs for game functions called by lane H's code that are not decompiled yet.

#include <xtl.h>
#include <xonline.h>

struct s_player_profile;
struct s_shapes;
union point3f;
struct s_controller_event;



struct s_event;
struct s_event_response;

struct s_dialog_definition;

struct s_screen_parameters;
class c_class_1473c9;

// @stub 0x18f42d
c_class_1473c9 *__stdcall function_18f42d(s_screen_parameters *parameters)
{
	return 0;
}

// @stub 0x18f474
c_class_1473c9 *__stdcall function_18f474(s_screen_parameters *parameters)
{
	return 0;
}




// @stub 0x59570
long function_59570(void)
{
	return 0;
}







// @stub 0xb3e90
void __stdcall function_b3e90(unsigned char *results)
{
}

// @stub 0x16a440
bool __stdcall function_16a440(unsigned long flags, point3f const *position, float extent, float height,
	float radius, long ignore_object, long ignore_parent, s_shapes *shapes)
{
	return false;
}

struct s_network_session_player;


// @stub 0x1391ed
void function_1391ed(void)
{
}

typedef void *(__stdcall *block_allocate)(void *, long, long);
typedef void (__stdcall *block_free)(void *, void *);

// @stub 0x2cb5b0
long __stdcall function_2cb5b0(unsigned char *destination, long *destination_size, unsigned char const *source, long source_size, block_allocate allocate, block_free release, void *opaque)
{
    return -1;
}

// @stub 0x2cb510
long __stdcall function_2cb510(unsigned char *destination, long *destination_size, unsigned char const *source, long source_size, long level, block_allocate allocate, block_free release, void *opaque)
{
    return -1;
}


// @stub 0x13954b
void function_13954b(void)
{
}

// @stub 0x191fd8
void function_191fd8(long player_index)
{
}
