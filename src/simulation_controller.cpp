// @flags /O2 /Ob1 /Gr
#include "unknown_11c920.h"
#include "unknown_067e10.h"
#include "network_message_types.h"
#include <string.h>

struct s_simulation_controller
{
	long field_00;
	long field_04;
	long field_08;
	t_player_key key;
	s_machine_address machine;
	byte field_1e[2];
	c_simulation_world *world;
	bool field_24;
	bool field_25;
	byte field_26[2];
	long field_28;
	s_player_action action;
};

void player_action_initialize(s_player_action *action);

static inline bool controller_world_is_authority(c_simulation_world *world)
{
	return world->state != 3 && world->state != 5;
}

// @retail 0x84750
void simulation_controller_initialize(s_simulation_controller *controller, c_simulation_world *world,
	long field_00, long field_04, long field_08, const s_machine_address *machine, const t_player_key *key)
{
	controller->field_00 = field_00;
	controller->field_04 = field_04;
	controller->field_08 = field_08;
	memcpy(controller->key, *key, sizeof(controller->key));
	controller->machine = *machine;
	controller->field_24 = false;
	controller->world = world;
	controller->field_25 = !controller_world_is_authority(world);
	controller->field_28 = NONE;
	player_action_initialize(&controller->action);
}
