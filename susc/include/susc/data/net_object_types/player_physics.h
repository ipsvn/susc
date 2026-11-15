#pragma once

#include "susc/data/net_object.h"

#include "susc/data/player_outfit_type.h"

#include "hazel/reader.h"
#include "hazel/writer.h"

typedef struct susc_no_player_physics
{
    susc_net_object base;

} susc_no_player_physics;

int susc_no_player_physics_instantiate(susc_net_object **net_object);
static struct susc_net_object_prefab susc_no_player_physics_type = {
	.instantiate = susc_no_player_physics_instantiate
};
