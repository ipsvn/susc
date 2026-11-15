#include "susc/data/net_object_types/lobby_behaviour.h"

#include "../../utils/utils.h"
#include "susc/data/enums/rpc_calls.h"

#include <malloc.h>

int susc_no_lobby_behaviour_deserialize(
	susc_net_object *net_object, susc_game_state *state, 
    hazel_message_reader *reader, bool initial_state)
{
    SUSC_UNUSED(net_object);
    SUSC_UNUSED(state);
    SUSC_UNUSED(reader);
    SUSC_UNUSED(initial_state);
    return 0;
}

int susc_no_lobby_behaviour_serialize(
	susc_net_object *net_object, susc_game_state *state, 
    hazel_message_writer *writer, bool initial_state)
{
    SUSC_UNUSED(net_object);
    SUSC_UNUSED(state);
    SUSC_UNUSED(writer);
    SUSC_UNUSED(initial_state);
    return SUSC_NET_OBJECT_NOT_SERIALIZABLE;
}

int susc_no_lobby_behaviour_handle_rpc(
	susc_net_object *net_object, susc_game_state *state,
    uint8_t call_id, hazel_message_reader *reader)
{
    switch (call_id)
    {
        case SUSC_RPC_CALLS_LOBBYTIMEEXPIRING:
        {
            break;
        }
        case SUSC_RPC_CALLS_EXTENDLOBBYTIMER:
        {

            break;
        }
    }

    return 0;
}

int susc_no_lobby_behaviour_on_spawn(
    susc_net_object *net_object, susc_game_state *game_state)
{
    return 0;
}


int susc_no_lobby_behaviour_instantiate(susc_net_object **net_object)
{
    susc_no_lobby_behaviour *obj = malloc(sizeof(susc_no_lobby_behaviour));
    obj->base._type = SUSC_NET_OBJECT_TYPE_LOBBY_BEHAVIOUR;
    obj->base._first = &obj->base;

    obj->base.deserialize = susc_no_lobby_behaviour_deserialize;
    obj->base.serialize = susc_no_lobby_behaviour_serialize;
    obj->base.handle_rpc = susc_no_lobby_behaviour_handle_rpc;
    obj->base.on_spawn = susc_no_lobby_behaviour_on_spawn;

    obj->base.destroy = susc_net_object_default_destroy;

    *net_object = (susc_net_object *) obj;
    
    return 0;
}
