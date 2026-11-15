#include "susc/data/net_object_types/network_transform.h"

#include "susc/game/events.h"
#include "susc/game/state.h"
#include "susc/utils/utils.h"

#include "susc/data/enums/rpc_calls.h"

#include "../../utils/utils.h"
#include "../../utils/hazel_utils.h"


#include <malloc.h>

int susc_no_network_transform_snap_to(
    susc_no_network_transform *obj, susc_game_state *state,
    susc_vector position, uint16_t min_sid)
{
    if (!susc_cmpsid_uint16(min_sid, obj->last_sequence_id))
    {
        return 0;
    }

    obj->last_sequence_id = min_sid;
    obj->position = position;

    susc_event_player_transform_snap *event = SUSC_CREATE_EVENT_OF_TYPE(
        susc_event_player_transform_snap, SUSC_EVENT_PLAYER_TRANSFORM_EVENT);
    event->base.player_transform_event_type = SUSC_PLAYER_TRANSFORM_EVENT_SNAP;
    event->base.transform = obj;
    event->position = position;
    susc_event_queue_push_event(&state->event_queue, (susc_event_base *)event);

    return 0;
}

int susc_no_network_transform_rpc_snap_to(
    susc_no_network_transform *obj, susc_game_state *state,
    susc_vector position)
{
    susc_no_network_transform_snap_to(obj, state, 
                                      position, obj->last_sequence_id + 1);

    SUSC_WRITER_STACK(writer, 128);
    SUSC_REQUIRE(susc_state_rpc_start(
        state, &writer, obj->base.net_id,
        SUSC_RPC_CALLS_SNAPTO));

    SUSC_REQUIRE(susc_write_vector(position, &writer));
    SUSC_REQUIRE(hazel_message_writer_uint16(
        &writer, obj->last_sequence_id + 2));

    SUSC_REQUIRE(susc_state_rpc_end(state, &writer));
    return 0;
}

int susc_no_network_transform_deserialize(
	susc_net_object *net_object, susc_game_state *game_state, 
    hazel_message_reader *reader, bool initial_state)
{
    susc_no_network_transform *network_transform =
        (susc_no_network_transform *)net_object;

    SUSC_LOG_DEBUG("player network_transform deserialize %p %d", 
                   (void *)net_object, net_object->net_id);

    if (initial_state)
    {
        SUSC_REQUIRE(hazel_message_reader_uint16(
            reader, &network_transform->last_sequence_id));
        SUSC_REQUIRE(susc_read_vector(
            &network_transform->position, reader));
    }
    else
    {
        if (net_object->owner_id == game_state->client_id)
        {
            return 0;
        }

        uint16_t num;
        SUSC_REQUIRE(hazel_message_reader_uint16(reader, &num));
        int32_t num2;
        SUSC_REQUIRE(hazel_message_reader_packed_int32(reader, &num2));

        for (int32_t i = 0; i < num2; i++)
        {
            uint16_t new_sid = (uint16_t)((int32_t)num + i);
            susc_vector new_position;
            SUSC_REQUIRE(susc_read_vector(&new_position, reader));
            if (new_sid > network_transform->last_sequence_id)
            {
                network_transform->position = new_position;
            }
        }
    }

    SUSC_LOG_DEBUG(
        "%d new player position: %f %f", 
        network_transform->base.net_id,
        network_transform->position.x,
        network_transform->position.y);

    return 0;
}

int susc_no_network_transform_serialize(
	susc_net_object *net_object, susc_game_state *game_state, 
    hazel_message_writer *message_writer, bool initial_state)
{
    // TODO
    return 0;
}

int susc_no_network_transform_handle_rpc(
	susc_net_object *net_object, susc_game_state *state,
    uint8_t call_id, hazel_message_reader *reader)
{
    susc_no_network_transform *no =
        (susc_no_network_transform *)net_object;
    
    if (call_id == 21)
    {
        susc_vector vec;
        SUSC_REQUIRE(susc_read_vector(&vec, reader));
        uint16_t min_sid;
        SUSC_REQUIRE(hazel_message_reader_uint16(reader, &min_sid));

        susc_no_network_transform_snap_to(no, state, vec, min_sid);
    }
    return 0;
}

int susc_no_network_transform_on_spawn(
    susc_net_object *net_object, susc_game_state *game_state)
{
    // TODO
    return 0;
}


int susc_no_network_transform_instantiate(susc_net_object **net_object)
{
    susc_no_network_transform *obj = malloc(sizeof(susc_no_network_transform));
    if (obj == NULL)
    {
        return SUSC_ERR_FAILED_ALLOC;
    }

    obj->base._type = SUSC_NET_OBJECT_TYPE_NETWORK_TRANSFORM;
    obj->base._first = &obj->base;

    obj->base.deserialize = susc_no_network_transform_deserialize;
    obj->base.serialize = susc_no_network_transform_serialize;
    obj->base.handle_rpc = susc_no_network_transform_handle_rpc;
    obj->base.on_spawn = susc_no_network_transform_on_spawn;

    obj->base.destroy = susc_net_object_default_destroy;

    *net_object = (susc_net_object *) obj;
    
    return 0;
}
