// @flags /O2 /arch:SSE /Gr
#include "unknown_11c920.h"
#include "globals.h"
#include "main_globals.h"
#include <xtl.h>

class c_class_93590;
struct s_save_list_2164c0;

extern long g_4ed294;
extern bool g_4ed39d;
extern bool g_55c14d;
extern void *g_51ea14;
extern long g_55c150;
extern bool g_510819;
extern bool g_55bd0c;
extern bool g_509340;
extern bool g_4d8ba0;
extern c_class_93590 *g_510560;
long g_4e28f0;

void function_2237d4(void);
void __stdcall function_18f1c0(long arg_0);
void function_18e810(void);
void function_12bad0(void);
void function_12ba30(void);
void function_12ba90(void);
void function_12bd40(void);
void function_12bb20(void);
void function_12bbd0(void);
void function_124380(char const *arg_0);
void function_12bdf0(void);
void function_124950(void);
void function_217c40(void);
void function_2164c0(long arg_0, s_save_list_2164c0 *arg_1);
void function_121120(void);
void font_cache_update(void);
long function_120bf0(void);
bool function_2141f0(void);
void map_load_request_update(void);
real function_12b0e0(void);
void __stdcall function_1479c3(real arg_0);
void function_155f80(bool arg_0);
void network_link_receive(c_class_93590 *arg_0);
void function_8df50(void);
void function_680f0(void);
void function_59940(void);
bool function_68250(void);
void function_146660(real arg_0, real *arg_1, long *arg_2);
void __stdcall function_1870a0(real arg_0, real arg_1);
void __fastcall function_137f70(long arg_0, real *arg_1);
void __stdcall function_137ed0(real arg_0);
void function_8dfc0(void);
void function_1554b0(real arg_0);
void __stdcall function_16f280(real arg_0);
void function_222a70(real arg_0);
void function_681e0(void);
void rumble_clear_all(void);
void function_223240(void);
void function_125b40(void);

#pragma inline_depth(0)
// @retail 0x12b450
void function_12b450(void)
{
    if (main_globals.unknown2a[0])
    {
        main_globals.unknown2a[0] = 0;
        function_2237d4();
    }
    if (g_4ed294 != 2 && g_4ed294 != 5 && !g_4ed39d)
        function_18f1c0(0);
    function_18e810();
    if (main_globals.unknown72)
        function_12bad0();
    if (main_globals.unknown6f)
        function_12ba30();
    if (main_globals.reset_map)
        function_12ba90();
    if (main_globals.switch_structure_bsp)
        function_12bd40();
    if (main_globals.save_map)
        function_12bb20();
    if (main_globals.quit_game)
        function_12bbd0();
    if (main_globals.field_5 && g_4e6948 && g_4e6948->flag1120)
    {
        function_124380(main_globals.field_6_2);
        main_globals.field_5 = 0;
    }
    if (main_globals.unknown76)
        function_12bdf0();
    function_124950();
    function_217c40();
    if (g_55c14d && g_51ea14 && (g_55c150 & 1))
    {
        function_2164c0(0, (s_save_list_2164c0 *)((byte *)g_51ea14 + 0xbef8));
        g_55c150 &= ~1;
    }
    if (g_510819)
        function_121120();
    font_cache_update();
    g_4e28f0 = function_120bf0();
    if (g_55bd0c)
        function_2141f0();
    map_load_request_update();
    if (!g_509340)
    {
        real local_0 = 0.0f;
        real local_3 = 0.0f;
        long local_1 = 0;
        real local_2 = function_12b0e0();
        local_3 = g_509340 ? 0.0f : local_2;
        function_1479c3(local_2);
        function_155f80(false);
        if (g_4d8ba0)
        {
            g_510548 = 1;
            g_51054c = GetTickCount();
            network_link_receive(g_510560);
            g_510548 = 0;
            g_51054c = GetTickCount();
        }
        function_8df50();
        function_680f0();
        function_59940();
        if (function_68250())
        {
            function_146660(local_3, &local_0, &local_1);
            function_1870a0(local_3, local_0);
            function_137f70(local_1, &local_0);
            function_8dfc0();
            function_137ed0(local_0);
            function_1554b0(local_3);
            function_16f280(local_3);
            function_222a70(local_3);
        }
        else
        {
            function_681e0();
            function_8dfc0();
            rumble_clear_all();
        }
        function_223240();
        function_125b40();
    }
}
#pragma inline_depth(255)
