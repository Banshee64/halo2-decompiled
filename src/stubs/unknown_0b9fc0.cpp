// stubs for game functions not decompiled yet, called by unknown_0b9fc0.cpp
#include "unknown_11c920.h"
#include "unknown_0259d0.h"

struct s_havok_component;
struct s_type_1a7926;
struct s_animation_frame_event;


/* moves a Havok component's bodies to a device position */
// @stub 0x1d24a0
void __stdcall function_1d24a0(s_havok_component *component, float position) { }
/* keyframes a Havok body to a matrix */
// @stub 0x1d0ee0
void function_1d0ee0(long rigid_body_index, s_havok_component *component, transform4x3f const *matrix) { }
/* a device's animation event callback */
// @stub 0xbf600
void __stdcall function_bf600(long user, float frame, s_animation_frame_event const *event) { }
/* an object's forward and up vectors */
/* the machine's and the crate's callback at +0x4c: asks the object's Havok
   component (another file's) */
// @stub 0x11bd60
bool __stdcall function_11bd60(long object_index, long a, long b, long c) { return false; }
