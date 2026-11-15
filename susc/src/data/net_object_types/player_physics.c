#include "susc/data/net_object_types/player_physics.h"

#include "../../utils/utils.h"

#include <malloc.h>

int susc_no_player_physics_deserialize(
	susc_net_object *net_object, susc_game_state *state, 
    hazel_message_reader *reader, bool initial_state)
{
    SUSC_LOG_DEBUG("player_physics deserialize")
    // TODO
    return 0;
}

int susc_no_player_physics_serialize(
	susc_net_object *net_object, susc_game_state *state, 
    hazel_message_writer *message_writer, bool initial_state)
{
    // TODO
    return 0;
}

int susc_no_player_physics_handle_rpc(
	susc_net_object *net_object, susc_game_state *state,
    uint8_t call_id, hazel_message_reader *reader)
{
    // TODO
    return 0;
}

int susc_no_player_physics_on_spawn(
    susc_net_object *net_object, susc_game_state *state)
{
    // TODO
    return 0;
}


int susc_no_player_physics_instantiate(susc_net_object **net_object)
{
    susc_no_player_physics *obj = malloc(sizeof(susc_no_player_physics));
    obj->base._type = SUSC_NET_OBJECT_TYPE_PLAYER_PHYSICS;
    obj->base._first = &obj->base;

    obj->base.deserialize = susc_no_player_physics_deserialize;
    obj->base.serialize = susc_no_player_physics_serialize;
    obj->base.handle_rpc = susc_no_player_physics_handle_rpc;
    obj->base.on_spawn = susc_no_player_physics_on_spawn;

    obj->base.destroy = susc_net_object_default_destroy;

    *net_object = (susc_net_object *) obj;
    
    return 0;
}
