#pragma once

#include "susc/data/net_object.h"

#include "susc/data/player_outfit_type.h"

#include "susc/utils/vector.h"

#include "hazel/reader.h"
#include "hazel/writer.h"

typedef struct susc_no_network_transform
{
    susc_net_object base;

    uint16_t last_sequence_id;
    susc_vector position;

} susc_no_network_transform;

int susc_no_network_transform_snap_to(
    susc_no_network_transform *obj, susc_game_state *state,
    susc_vector position, uint16_t min_sid);

int susc_no_network_transform_rpc_snap_to(
    susc_no_network_transform *obj, susc_game_state *state,
    susc_vector position);

int susc_no_network_transform_instantiate(susc_net_object **net_object);
static struct susc_net_object_prefab susc_no_network_transform_type = {
	.instantiate = susc_no_network_transform_instantiate
};
