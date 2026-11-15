#include "state_internal.h"

#include "net_object_container.h"
#include "gamedata_flag.h"
#include "../utils/hazel_utils.h"
#include "susc/data/net_object.h"

#include <malloc.h>

int susc_state_write_spawn_message(
    susc_game_state *state, susc_net_object *obj, 
    int owner_id, enum susc_spawn_flags flags,
    hazel_message_writer *writer)
{
    SUSC_REQUIRE(hazel_message_writer_start_message(
        writer, SUSC_GAMEDATA_FLAG_SPAWN));
    SUSC_REQUIRE(hazel_message_writer_packed_uint32(writer, obj->spawn_type));
    SUSC_REQUIRE(hazel_message_writer_packed_int32(writer, owner_id));
    SUSC_REQUIRE(hazel_message_writer_uint8(writer, flags));

    SUSC_NO_ITERATE_CHILDREN(obj, child)
    {
        SUSC_REQUIRE(hazel_message_writer_packed_uint32(
            writer, child->net_id));
        SUSC_REQUIRE(hazel_message_writer_start_message(
            writer, SUSC_GAMEDATA_FLAG_DATA));
        SUSC_REQUIRE(child->serialize(child, state, writer, true));
        SUSC_REQUIRE(hazel_message_writer_end_message(writer));
    }

    SUSC_REQUIRE(hazel_message_writer_end_message(writer));
    return 0;
}

int susc_state_handle_despawn_message(
    susc_game_state *state, uint32_t net_id)
{    
    susc_net_object_container *container =
        susc_state_no_find_container(state, net_id);
    if (container == NULL)
    {
        susc_state_no_insert_netid(state, net_id, NULL);
        return 0;
    }

    if (container->obj == NULL)
    {
        SUSC_LOG_DEBUG("Attempt to despawn previously despawned object");
        return 0;
    }

    if (container->obj->owner_id != state->client_id)
    {
        container->obj->net_id = UINT32_MAX;
        // TODO free the memory of obj
        free(container->obj);
    }

    container->obj = NULL;

    return 0;
}

