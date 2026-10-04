// @flags /O2 /Ob1 /Gr /arch:SSE
/* Legacy recorded animation streams. Existing field readers remain in
   unknown_29f180.cpp with their upstream definitions. */
#include "cseries.h"

struct animation_playback_controller;
struct playback_unit_control_view;

struct animation_event_v1
{
    short type;
    unsigned short ticks;
};

void __stdcall function_29f180(byte *dest, byte *src, byte **cursor);
void __stdcall function_29f1a0(byte *dest, byte *src, byte **cursor);
void __stdcall function_29f1c0(byte *dest, byte *src, byte **cursor);
void __stdcall function_29f1e0(byte *dest, byte *src, byte **cursor);
void __stdcall function_29f200(byte *dest, byte *src, byte **cursor);
void __stdcall function_29f220(byte *dest, real *src, byte **cursor);
void __stdcall function_29f250(byte *dest, byte *src, byte **cursor);
void __stdcall function_29f280(byte *dest, byte *src, byte **cursor);
void __stdcall function_29f2b0(byte *dest, byte *src, byte **cursor);
void __stdcall function_29f2e0(byte *dest, word *src, byte **cursor);
void __stdcall function_29f370(byte *dest, word *src, byte **cursor);

void recorded_animation_initialize_unit_control(playback_unit_control_view *control,
    byte const **cursor, byte version);

typedef void (__stdcall *legacy_event_handler)(playback_unit_control_view *,
    animation_event_v1 const *, byte const **);

legacy_event_handler const g_471220[24] =
{
    NULL,
    NULL,
    (legacy_event_handler)function_29f180,
    (legacy_event_handler)function_29f1a0,
    (legacy_event_handler)function_29f1c0,
    (legacy_event_handler)function_29f1e0,
    (legacy_event_handler)function_29f220,
    NULL,
    NULL,
    (legacy_event_handler)function_29f250,
    (legacy_event_handler)function_29f280,
    (legacy_event_handler)function_29f2b0,
    (legacy_event_handler)function_29f370,
    (legacy_event_handler)function_29f370,
    (legacy_event_handler)function_29f370,
    (legacy_event_handler)function_29f370,
    (legacy_event_handler)function_29f200,
    (legacy_event_handler)function_29f2e0,
    (legacy_event_handler)function_29f2e0,
    (legacy_event_handler)function_29f2e0,
    (legacy_event_handler)function_29f2e0,
    (legacy_event_handler)function_29f2e0,
    (legacy_event_handler)function_29f2e0,
    (legacy_event_handler)function_29f2e0
};

// @retail 0x29f3e0
void __stdcall recorded_animation_initialize_event_stream_v1(animation_playback_controller *controller,
    playback_unit_control_view *control, byte const **cursor, byte version)
{
    recorded_animation_initialize_unit_control(control, cursor, version);
}

// @retail 0x29f400
bool __stdcall recorded_animation_apply_event_stream_v1(animation_playback_controller *controller,
    playback_unit_control_view *control, long *remaining_ticks, byte const **cursor)
{
    animation_event_v1 const *event = (animation_event_v1 const *)*cursor;
    while (*remaining_ticks >= event->ticks)
    {
        if (event->type == 1)
            break;
        legacy_event_handler handler = g_471220[event->type];
        if (handler)
            handler(control, event, cursor);
        else
            *cursor = (byte const *)(event + 1);
        *remaining_ticks -= event->ticks;
        event = (animation_event_v1 const *)*cursor;
    }
    return event->type != 1 || *remaining_ticks != event->ticks;
}

struct legacy_playback_functions
{
    void (__stdcall *initialize)(animation_playback_controller *, playback_unit_control_view *,
        byte const **, byte);
    bool (__stdcall *apply)(animation_playback_controller *, playback_unit_control_view *,
        long *, byte const **);
};

// Retail's legacy codec pair; the newer format's pair is at 0x46fd4c.
legacy_playback_functions const g_46fd54 =
{
    recorded_animation_initialize_event_stream_v1,
    recorded_animation_apply_event_stream_v1
};
