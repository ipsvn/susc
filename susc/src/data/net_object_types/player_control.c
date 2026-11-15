#include "susc/data/net_object_types/player_control.h"

#include "susc/data/net_object_types/network_transform.h"
#include "susc/data/net_object_types/player_physics.h"
#include "susc/data/enums/rpc_calls.h"

#include "susc/game/events.h"
#include "susc/game/state.h"

#include "../../utils/hazel_utils.h"
#include "../../utils/utils.h"

#include <malloc.h>

int susc_no_player_control_cmd_check_name(
    susc_no_player_control *obj, susc_game_state *state,
    const char *name)
{
    SUSC_WRITER_STACK(writer, 128);
    SUSC_REQUIRE(susc_state_rpc_start_to(
        state, &writer, obj->base.net_id, state->host_id,
        SUSC_RPC_CALLS_CHECKNAME));

    SUSC_REQUIRE(hazel_message_writer_string(&writer, name));

    SUSC_REQUIRE(susc_state_rpc_end(state, &writer));
    return 0;
}

int susc_no_player_control_cmd_check_color(
    susc_no_player_control *obj, susc_game_state *state,
    enum susc_player_color color)
{
    SUSC_WRITER_STACK(writer, 128);
    SUSC_REQUIRE(susc_state_rpc_start_to(
        state, &writer, obj->base.net_id, state->host_id,
        SUSC_RPC_CALLS_CHECKCOLOR));

    SUSC_REQUIRE(hazel_message_writer_uint8(&writer, color));

    SUSC_REQUIRE(susc_state_rpc_end(state, &writer));
    return 0;
}

int susc_no_player_control_rpc_set_level(
    susc_no_player_control* obj, susc_game_state* state,
    uint32_t level)
{
    // todo i actually need to run SetLevel client sided
    SUSC_WRITER_STACK(writer, 128);
    SUSC_REQUIRE(susc_state_rpc_start(
        state, &writer, obj->base.net_id,
        SUSC_RPC_CALLS_SETLEVEL));

    SUSC_REQUIRE(hazel_message_writer_packed_uint32(&writer, level));

    SUSC_REQUIRE(susc_state_rpc_end(state, &writer));
    return 0;
}

int susc_no_player_control_rpc_set_pet(
    susc_no_player_control* obj, susc_game_state* state,
    char* pet_id)
{
    // todo i actually need to run SetLevel client sided
    SUSC_WRITER_STACK(writer, 128);
    SUSC_REQUIRE(susc_state_rpc_start(
        state, &writer, obj->base.net_id,
        SUSC_RPC_CALLS_SETPETSTR));

    SUSC_REQUIRE(hazel_message_writer_string(&writer, pet_id));
    SUSC_REQUIRE(hazel_message_writer_uint8(&writer, 69)); // todo GetNextRpcSequenceId

    SUSC_REQUIRE(susc_state_rpc_end(state, &writer));
    return 0;
}

int susc_no_player_control_rpc_set_hat(
    susc_no_player_control* obj, susc_game_state* state,
    char* hat_id)
{
    // todo i actually need to run SetLevel client sided
    SUSC_WRITER_STACK(writer, 128);
    SUSC_REQUIRE(susc_state_rpc_start(
        state, &writer, obj->base.net_id,
        SUSC_RPC_CALLS_SETHATSTR));

    SUSC_REQUIRE(hazel_message_writer_string(&writer, hat_id));
    SUSC_REQUIRE(hazel_message_writer_uint8(&writer, 69)); // todo GetNextRpcSequenceId

    SUSC_REQUIRE(susc_state_rpc_end(state, &writer));
    return 0;
}

int susc_no_player_control_rpc_set_skin(
    susc_no_player_control* obj, susc_game_state* state,
    char* skin_id)
{
    // todo i actually need to run SetLevel client sided
    SUSC_WRITER_STACK(writer, 128);
    SUSC_REQUIRE(susc_state_rpc_start(
        state, &writer, obj->base.net_id,
        SUSC_RPC_CALLS_SETSKINSTR));

    SUSC_REQUIRE(hazel_message_writer_string(&writer, skin_id));
    SUSC_REQUIRE(hazel_message_writer_uint8(&writer, 69)); // todo GetNextRpcSequenceId

    SUSC_REQUIRE(susc_state_rpc_end(state, &writer));
    return 0;
}

int susc_no_player_control_rpc_set_visor(
    susc_no_player_control* obj, susc_game_state* state,
    char* visor_id)
{
    // todo i actually need to run SetLevel client sided
    SUSC_WRITER_STACK(writer, 128);
    SUSC_REQUIRE(susc_state_rpc_start(
        state, &writer, obj->base.net_id,
        SUSC_RPC_CALLS_SETVISORSTR));

    SUSC_REQUIRE(hazel_message_writer_string(&writer, visor_id));
    SUSC_REQUIRE(hazel_message_writer_uint8(&writer, 69)); // todo GetNextRpcSequenceId

    SUSC_REQUIRE(susc_state_rpc_end(state, &writer));
    return 0;
}

int susc_no_player_control_rpc_set_nameplate(
    susc_no_player_control* obj, susc_game_state* state,
    char* nameplate_id)
{
    // todo i actually need to run SetLevel client sided
    SUSC_WRITER_STACK(writer, 128);
    SUSC_REQUIRE(susc_state_rpc_start(
        state, &writer, obj->base.net_id,
        SUSC_RPC_CALLS_SETNAMEPLATESTR));

    SUSC_REQUIRE(hazel_message_writer_string(&writer, nameplate_id));
    SUSC_REQUIRE(hazel_message_writer_uint8(&writer, 69)); // todo GetNextRpcSequenceId

    SUSC_REQUIRE(susc_state_rpc_end(state, &writer));
    return 0;
}

int susc_no_player_control_rpc_send_chat(
    susc_no_player_control *obj, susc_game_state *state,
    const char *message)
{
    // TODO? so there seemingly isn't a hard limit on the actual length, but its
    // effectively limited by the packet size since hazel won't fragment it...
    SUSC_WRITER_STACK(writer, 512);
    SUSC_REQUIRE(susc_state_rpc_start(
        state, &writer, obj->base.net_id,
        SUSC_RPC_CALLS_SENDCHAT));

    SUSC_REQUIRE(hazel_message_writer_string(&writer, message));

    SUSC_REQUIRE(susc_state_rpc_end(state, &writer));
    return 0;
}

int susc_no_player_control_deserialize(
    susc_net_object *net_object, susc_game_state *game_state,
    hazel_message_reader *reader, bool initial_state)
{
    susc_no_player_control *pc = (susc_no_player_control *) net_object;

    if (initial_state)
    {
        SUSC_REQUIRE(hazel_message_reader_bool(reader, &pc->is_new));
    }
    SUSC_REQUIRE(hazel_message_reader_uint8(reader, &pc->player_id));

    return 0;
}

int susc_no_player_control_serialize(
    susc_net_object *net_object, susc_game_state *game_state,
    hazel_message_writer *writer, bool initial_state)
{
    susc_no_player_control *pc = (susc_no_player_control *) net_object;

    if (initial_state)
    {
        SUSC_REQUIRE(hazel_message_writer_bool(writer, pc->is_new));
    }
    SUSC_REQUIRE(hazel_message_writer_uint8(writer, pc->player_id));

    return 0;
}

int susc_no_player_control_handle_rpc(
    susc_net_object *net_object, susc_game_state *game_state,
    uint8_t call_id, hazel_message_reader *reader)
{
    susc_no_player_control *pc = (susc_no_player_control *) net_object;
    switch (call_id)
    {
        case SUSC_RPC_CALLS_SENDCHAT:
        {
            char *message;
            SUSC_REQUIRE(hazel_message_reader_string_malloc(reader, &message));

            susc_event_player_rpc_send_chat *event 
                = malloc(sizeof(susc_event_player_rpc_send_chat));
            event->base.base.type = SUSC_EVENT_PLAYER_EVENT;
            event->base.player_event_type = SUSC_PLAYER_EVENT_RPC_SEND_CHAT;
            event->base.player = pc;
            event->message = message;
            susc_event_queue_push_event(&game_state->event_queue, (susc_event_base *)event);

            break;
        }
    }
    return 0;
}

int susc_no_player_control_on_spawn(
    susc_net_object *net_object, susc_game_state *game_state)
{
    // TODO
    return 0;
}

susc_no_player_physics *susc_no_player_control_get_physics(
    susc_no_player_control *obj)
{
    return (susc_no_player_physics *)susc_net_object_find_child_of_type(
        &obj->base, SUSC_NET_OBJECT_TYPE_PLAYER_PHYSICS);
}

susc_no_network_transform *susc_no_player_control_get_transform(
    susc_no_player_control *obj)
{
    return (susc_no_network_transform *)susc_net_object_find_child_of_type(
        &obj->base, SUSC_NET_OBJECT_TYPE_NETWORK_TRANSFORM);
}


int susc_no_player_control_instantiate(susc_net_object **net_object)
{
    susc_no_player_control *obj = malloc(sizeof(susc_no_player_control));
    obj->base._type = SUSC_NET_OBJECT_TYPE_PLAYER_CONTROL;
    obj->base._first = &obj->base;

    obj->base.deserialize = susc_no_player_control_deserialize;
    obj->base.serialize = susc_no_player_control_serialize;
    obj->base.handle_rpc = susc_no_player_control_handle_rpc;
    obj->base.on_spawn = susc_no_player_control_on_spawn;

    obj->base.destroy = susc_net_object_default_destroy;

    obj->is_new = true;
    obj->player_id = UINT8_MAX;

    susc_net_object *network_transform;
    SUSC_REQUIRE(susc_no_network_transform_instantiate(&network_transform));
    network_transform->_first = &obj->base;

    susc_net_object *player_physics;
    SUSC_REQUIRE(susc_no_player_physics_instantiate(&player_physics));
    player_physics->_first = &obj->base;

    obj->base._next = player_physics;
    player_physics->_next = network_transform;

    *net_object = (susc_net_object *)obj;

    return 0;
}
