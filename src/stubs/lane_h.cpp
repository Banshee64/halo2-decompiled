// stubs for the game functions outside 0x190000..0x19ffff that lane H's code
// calls and that are not decompiled yet

#include <xtl.h>
#include <xonline.h>

struct s_player_profile;
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


// @stub 0xb3610
bool function_b3610(void)
{
	return false;
}

// @stub 0xb3670
void function_b3670(void)
{
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

// @stub 0x19bfd0
bool __stdcall function_19bfd0(struct s_content_item *item)
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
