// @flags /O2 /Gr
/* UNKNOWN_0E6900.CPP: sending a request to a unit (an outside function lane A's
   AI script functions need; its callers are in many regions) */

#include "cseries.h"
#include "globals.h"
#include "slot_handler.h"

/* a unit request type's handlers (0x467564..0x4677b4, 0x10 bytes each, in
   0xe7280..0xedf60): the first performs the request */
typedef bool (__stdcall *t_unit_request_proc)(long unit_index, s_unit_request *request);

struct s_unit_request_definition
{
	t_unit_request_proc perform;
	t_unit_request_proc unknown04;
	t_unit_request_proc unknown08;
	t_unit_request_proc unknown0c;
};

/* the callbacks (stubs in src/stubs/lane_i.cpp) */
bool __stdcall function_e7280(long unit_index, s_unit_request *request);
bool __stdcall function_e7320(long unit_index, s_unit_request *request);
bool __stdcall function_e73c0(long unit_index, s_unit_request *request);
bool __stdcall function_e7440(long unit_index, s_unit_request *request);
bool __stdcall function_e7fb0(long unit_index, s_unit_request *request);
bool __stdcall function_e8380(long unit_index, s_unit_request *request);
bool __stdcall function_e8420(long unit_index, s_unit_request *request);
bool __stdcall function_e8980(long unit_index, s_unit_request *request);
bool __stdcall function_e8b20(long unit_index, s_unit_request *request);
bool __stdcall function_e8c10(long unit_index, s_unit_request *request);
bool __stdcall function_e8d10(long unit_index, s_unit_request *request);
bool __stdcall function_e8de0(long unit_index, s_unit_request *request);
bool __stdcall function_e9000(long unit_index, s_unit_request *request);
bool __stdcall function_e9190(long unit_index, s_unit_request *request);
bool __stdcall function_e9370(long unit_index, s_unit_request *request);
bool __stdcall function_e9500(long unit_index, s_unit_request *request);
bool __stdcall function_e9690(long unit_index, s_unit_request *request);
bool __stdcall function_e9830(long unit_index, s_unit_request *request);
bool __stdcall function_e9ed0(long unit_index, s_unit_request *request);
bool __stdcall function_ea090(long unit_index, s_unit_request *request);
bool __stdcall function_ea1c0(long unit_index, s_unit_request *request);
bool __stdcall function_ea6b0(long unit_index, s_unit_request *request);
bool __stdcall function_ea7c0(long unit_index, s_unit_request *request);
bool __stdcall function_ea830(long unit_index, s_unit_request *request);
bool __stdcall function_eab80(long unit_index, s_unit_request *request);
bool __stdcall function_eae60(long unit_index, s_unit_request *request);
bool __stdcall function_eaea0(long unit_index, s_unit_request *request);
bool __stdcall function_eaeb0(long unit_index, s_unit_request *request);
bool __stdcall function_eb090(long unit_index, s_unit_request *request);
bool __stdcall function_eb270(long unit_index, s_unit_request *request);
bool __stdcall function_eb340(long unit_index, s_unit_request *request);
bool __stdcall function_eb3c0(long unit_index, s_unit_request *request);
bool __stdcall function_eb520(long unit_index, s_unit_request *request);
bool __stdcall function_eb5a0(long unit_index, s_unit_request *request);
bool __stdcall function_eb5d0(long unit_index, s_unit_request *request);
bool __stdcall function_eb7e0(long unit_index, s_unit_request *request);
bool __stdcall function_eb960(long unit_index, s_unit_request *request);
bool __stdcall function_ebaa0(long unit_index, s_unit_request *request);
bool __stdcall function_ebb40(long unit_index, s_unit_request *request);
bool __stdcall function_ebd30(long unit_index, s_unit_request *request);
bool __stdcall function_ebf20(long unit_index, s_unit_request *request);
bool __stdcall function_ec050(long unit_index, s_unit_request *request);
bool __stdcall function_ec2b0(long unit_index, s_unit_request *request);
bool __stdcall function_ec2d0(long unit_index, s_unit_request *request);
bool __stdcall function_ec330(long unit_index, s_unit_request *request);
bool __stdcall function_ec380(long unit_index, s_unit_request *request);
bool __stdcall function_ec4f0(long unit_index, s_unit_request *request);
bool __stdcall function_ec940(long unit_index, s_unit_request *request);
bool __stdcall function_ec970(long unit_index, s_unit_request *request);
bool __stdcall function_ec9a0(long unit_index, s_unit_request *request);
bool __stdcall function_ecb20(long unit_index, s_unit_request *request);
bool __stdcall function_ecb80(long unit_index, s_unit_request *request);
bool __stdcall function_ecc70(long unit_index, s_unit_request *request);
bool __stdcall function_eccc0(long unit_index, s_unit_request *request);
bool __stdcall function_ecd50(long unit_index, s_unit_request *request);
bool __stdcall function_ecdc0(long unit_index, s_unit_request *request);
bool __stdcall function_ecf30(long unit_index, s_unit_request *request);
bool __stdcall function_ecfc0(long unit_index, s_unit_request *request);
bool __stdcall function_ecff0(long unit_index, s_unit_request *request);
bool __stdcall function_ed560(long unit_index, s_unit_request *request);
bool __stdcall function_ed680(long unit_index, s_unit_request *request);
bool __stdcall function_ed710(long unit_index, s_unit_request *request);
bool __stdcall function_ed800(long unit_index, s_unit_request *request);
bool __stdcall function_ed910(long unit_index, s_unit_request *request);
bool __stdcall function_eda30(long unit_index, s_unit_request *request);
bool __stdcall function_ede60(long unit_index, s_unit_request *request);
bool __stdcall function_edf10(long unit_index, s_unit_request *request);
bool __stdcall function_edf60(long unit_index, s_unit_request *request);

/* folded in retail with the other empty callbacks of two arguments */
static bool __stdcall unit_request_ignore(long unit_index, s_unit_request *request)
{
	return false;
}

s_unit_request_definition g_467564 = {function_e7280, function_e7320, 0, function_e73c0};
s_unit_request_definition g_467574 = {function_e7440, 0, 0, 0};
s_unit_request_definition g_467584 = {function_e8980, function_e8b20, function_e8d10, function_e8c10};
s_unit_request_definition g_467594 = {function_e8de0, 0, 0, 0};
s_unit_request_definition g_4675a4 = {function_e9000, 0, 0, 0};
s_unit_request_definition g_4675b4 = {function_e9190, 0, 0, 0};
s_unit_request_definition g_4675c4 = {function_e7fb0, function_e8380, 0, function_e8420};
s_unit_request_definition g_4675d4 = {function_e9370, 0, 0, 0};
s_unit_request_definition g_4675e4 = {function_e9500, 0, 0, 0};
s_unit_request_definition g_4675f4 = {function_e9690, 0, 0, 0};
s_unit_request_definition g_467604 = {function_e9830, 0, 0, 0};
s_unit_request_definition g_467614 = {function_e9ed0, function_ea090, 0, function_ea1c0};
s_unit_request_definition g_467624 = {function_ea830, function_ea7c0, function_ea6b0, 0};
s_unit_request_definition g_467634 = {function_eab80, function_eaeb0, function_eae60, 0};
s_unit_request_definition g_467644 = {function_eaea0, 0, 0, 0};
s_unit_request_definition g_467654 = {function_eb090, function_eb340, function_eb270, 0};
s_unit_request_definition g_467664 = {function_eb3c0, function_eb5a0, function_eb520, unit_request_ignore};
s_unit_request_definition g_467674 = {function_eb5d0, 0, 0, 0};
s_unit_request_definition g_467684 = {function_eb7e0, 0, function_eb960, 0};
s_unit_request_definition g_467694 = {function_ebaa0, 0, function_ec2b0, 0};
s_unit_request_definition g_4676a4 = {function_ebb40, 0, 0, 0};
s_unit_request_definition g_4676b4 = {function_ebd30, 0, 0, 0};
s_unit_request_definition g_4676c4 = {function_ebf20, 0, 0, 0};
s_unit_request_definition g_4676d4 = {function_ec050, 0, function_ec2b0, 0};
s_unit_request_definition g_4676e4 = {function_ec2d0, 0, function_ec330, 0};
s_unit_request_definition g_4676f4 = {function_ec380, function_ec4f0, 0, 0};
s_unit_request_definition g_467704 = {function_ec940, 0, function_ec2b0, 0};
s_unit_request_definition g_467714 = {function_ec970, 0, function_ec2b0, 0};
s_unit_request_definition g_467724 = {function_ec9a0, 0, function_ecb20, 0};
s_unit_request_definition g_467734 = {function_eccc0, 0, 0, 0};
s_unit_request_definition g_467744 = {function_ecd50, 0, function_ecdc0, 0};
s_unit_request_definition g_467754 = {function_ecb80, 0, function_ecc70, 0};
s_unit_request_definition g_467764 = {function_ecf30, 0, function_ecfc0, 0};
s_unit_request_definition g_467774 = {function_ecff0, function_ed560, 0, 0};
s_unit_request_definition g_467784 = {function_ed680, function_ed710, 0, 0};
s_unit_request_definition g_467794 = {function_ed800, function_ed910, 0, 0};
s_unit_request_definition g_4677a4 = {function_eda30, 0, 0, 0};
s_unit_request_definition g_4677b4 = {function_ede60, function_edf10, function_edf60, 0};

/* the definition of each request type (60 types, in .data) */
s_unit_request_definition *g_4677c8[60] =
{
	&g_467564, &g_467564, &g_467574, &g_467574, &g_467574, &g_467574,
	&g_467574, &g_467574, &g_467584, &g_467594, &g_467564, &g_467564,
	&g_467574, &g_467574, &g_467574, &g_467574, &g_467574, &g_467574,
	&g_467584, &g_467594, &g_4675a4, &g_4675b4, &g_4675c4, &g_4675d4,
	&g_4675e4, &g_4675f4, &g_467604, &g_467614, &g_467624, &g_467634,
	&g_467644, &g_467654, &g_467664, &g_467674, &g_467684, &g_467694,
	&g_4676a4, &g_4676b4, &g_4676c4, &g_4676d4, &g_4676e4, &g_4676f4,
	&g_467704, &g_467714, &g_467724, &g_467734, &g_467744, &g_467754,
	&g_467764, &g_467774, &g_467784, &g_467794, &g_4677a4, &g_4677b4,
	&g_467574, &g_467574, &g_467574, &g_467574, &g_467574, &g_467574
};

void function_b7360(long object_index);
void function_1e77c0(long player_index, long type, byte result);

// @retail 0xe6900
bool function_e6900(long unit_index, s_unit_request *request)
{
	s_unit_request_definition *definition = g_4677c8[request->type];
	bool result;

	function_b7360(unit_index);
	result = definition->perform(unit_index, request);

	if (unit_index != NONE)
	{
		s_slot_object_view *unit = object_get(unit_index);

		if (unit->player_index != NONE)
			function_1e77c0(unit->player_index, request->type, result);
	}

	return result;
}
