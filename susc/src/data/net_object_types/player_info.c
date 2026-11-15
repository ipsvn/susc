#include "susc/data/net_object_types/player_info.h"

#include "susc/game/events.h"
#include "susc/game/state.h"
#include "susc/utils/utils.h"

#include "susc/data/enums/role_types.h"
#include "susc/data/enums/rpc_calls.h"

#include "../../utils/utils.h"
#include "../../utils/hazel_utils.h"

#include "hazel/reader.h"
#include "hazel/writer.h"

#include <string.h>
#include <malloc.h>

struct susc_no_player_info_task_info_list
{
    susc_no_player_info_task_info task;
    struct susc_no_player_info_task_info_list *next;
};

int susc_no_player_info_task_info_serialize(
    susc_no_player_info_task_info *task_info,
    hazel_message_writer *writer)
{
    SUSC_REQUIRE(hazel_message_writer_packed_uint32(writer, task_info->id));
    SUSC_REQUIRE(hazel_message_writer_bool(writer, task_info->complete));
    return 0;
}

int susc_no_player_info_task_info_deserialize(
    susc_no_player_info_task_info *task_info,
    hazel_message_reader *reader)
{
    SUSC_REQUIRE(hazel_message_reader_packed_uint32(reader, &task_info->id));
    SUSC_REQUIRE(hazel_message_reader_bool(reader, &task_info->complete));
    return 0;
}


int susc_no_player_info_outfit_serialize(
    susc_no_player_info_outfit *outfit,
    hazel_message_writer *writer)
{
    SUSC_REQUIRE(hazel_message_writer_string(writer, outfit->player_name));
    SUSC_REQUIRE(hazel_message_writer_packed_int32(writer, outfit->color_id));
    SUSC_REQUIRE(hazel_message_writer_string(writer, outfit->hat_id));
    SUSC_REQUIRE(hazel_message_writer_string(writer, outfit->pet_id));
    SUSC_REQUIRE(hazel_message_writer_string(writer, outfit->skin_id));
    SUSC_REQUIRE(hazel_message_writer_string(writer, outfit->visor_id));
    SUSC_REQUIRE(hazel_message_writer_string(writer, outfit->name_plate_id));
    SUSC_REQUIRE(hazel_message_writer_uint8(writer, outfit->hat_sequence_id));
    SUSC_REQUIRE(hazel_message_writer_uint8(writer, outfit->pet_sequence_id));
    SUSC_REQUIRE(hazel_message_writer_uint8(writer, outfit->skin_sequence_id));
    SUSC_REQUIRE(hazel_message_writer_uint8(writer, outfit->visor_sequence_id));
    SUSC_REQUIRE(hazel_message_writer_uint8(writer, outfit->name_plate_sequence_id));
    return 0;
}

int susc_no_player_info_outfit_deserialize(
    susc_no_player_info_outfit *outfit,
    hazel_message_reader *reader)
{
    SUSC_REQUIRE(hazel_message_reader_string_malloc(reader, &outfit->player_name));
    SUSC_REQUIRE(hazel_message_reader_packed_int32(reader, &outfit->color_id));
    SUSC_REQUIRE(hazel_message_reader_string_malloc(reader, &outfit->hat_id));
    SUSC_REQUIRE(hazel_message_reader_string_malloc(reader, &outfit->pet_id));
    SUSC_REQUIRE(hazel_message_reader_string_malloc(reader, &outfit->skin_id));
    SUSC_REQUIRE(hazel_message_reader_string_malloc(reader, &outfit->visor_id));
    SUSC_REQUIRE(hazel_message_reader_string_malloc(reader, &outfit->name_plate_id));
    SUSC_REQUIRE(hazel_message_reader_uint8(reader, &outfit->hat_sequence_id));
    SUSC_REQUIRE(hazel_message_reader_uint8(reader, &outfit->pet_sequence_id));
    SUSC_REQUIRE(hazel_message_reader_uint8(reader, &outfit->skin_sequence_id));
    SUSC_REQUIRE(hazel_message_reader_uint8(reader, &outfit->visor_sequence_id));
    SUSC_REQUIRE(hazel_message_reader_uint8(reader, &outfit->name_plate_sequence_id));
    return 0;
}



int susc_no_player_info_instantiate(susc_net_object **net_object)
{
    susc_no_player_info *obj = calloc(1, sizeof(susc_no_player_info));
    if (obj == NULL)
    {
        return SUSC_ERR_FAILED_ALLOC;
    }
    obj->base._type = SUSC_NET_OBJECT_TYPE_PLAYER_INFO;
    obj->base._first = &obj->base;

    obj->base.deserialize = susc_no_player_info_deserialize;
    obj->base.serialize = susc_no_player_info_serialize;
    obj->base.handle_rpc = susc_no_player_info_handle_rpc;
    obj->base.on_spawn = susc_no_player_info_on_spawn;

    obj->base.destroy = susc_net_object_default_destroy;

    *net_object = (susc_net_object *) obj;
    
    return 0;
}

int susc_no_player_info_deserialize(
	susc_net_object *net_object, susc_game_state *state, 
    hazel_message_reader *reader, bool initial_state)
{
    susc_no_player_info *no = (susc_no_player_info *)net_object;

    SUSC_REQUIRE(hazel_message_reader_uint8(reader, &no->player_id));
    SUSC_REQUIRE(hazel_message_reader_packed_int32(reader, &no->client_id));

    // Clear outfits
    for (int i = 0; i < SUSC_PLAYER_OUTFIT_COUNT; i++)
    {
        if (no->outfits[i] != NULL)
        {
            free(no->outfits[i]);
        }
        no->outfits[i] = NULL;
    }

    uint8_t outfit_count;
    SUSC_REQUIRE(hazel_message_reader_uint8(reader, &outfit_count));
    
    for (uint8_t i = 0; i < outfit_count; i++)
    {
        uint8_t outfit_type;
        SUSC_REQUIRE(hazel_message_reader_uint8(reader, &outfit_type));
        if (outfit_type >= SUSC_PLAYER_OUTFIT_COUNT)
        {
            // TODO error code
            return -1;
        }

        susc_no_player_info_outfit* outfit = malloc(sizeof(susc_no_player_info_outfit));
        susc_no_player_info_outfit_deserialize(outfit, reader);
        no->outfits[outfit_type] = outfit;
    }

    SUSC_REQUIRE(hazel_message_reader_packed_uint32(reader, &no->player_level));

    uint8_t flags;
    SUSC_REQUIRE(hazel_message_reader_uint8(reader, &flags));
    no->disconnected = (flags & 1) > 0;
    no->is_dead = (flags & 4) > 0;

    SUSC_READER_TYPECONV_REQUIRE(reader, uint16, no->role_type);

    bool has_role_when_alive;
    SUSC_REQUIRE(hazel_message_reader_bool(reader, &has_role_when_alive));
    if (has_role_when_alive)
    {
        SUSC_READER_TYPECONV_REQUIRE(reader, uint16, no->role_when_alive);
    }

    uint8_t task_count;
    SUSC_REQUIRE(hazel_message_reader_uint8(reader, &task_count));

    while (no->tasks != NULL)
    {
        struct susc_no_player_info_task_info_list *tmp = no->tasks;
        no->tasks = no->tasks->next;
        free(tmp);
    }

    struct susc_no_player_info_task_info_list *task_tail = NULL;
    for (uint8_t i = 0; i < task_count; i++)
    {
        struct susc_no_player_info_task_info_list *task_node 
            = malloc(sizeof(struct susc_no_player_info_task_info_list));

        if (no->tasks == NULL)
        {
            no->tasks = task_node;
        }
        else
        {
            task_tail->next = task_node;
        }

        task_tail = task_node;

        susc_no_player_info_task_info_deserialize(&task_node->task, reader);
    }

    SUSC_REQUIRE(hazel_message_reader_string(
        reader, no->friend_code, sizeof(no->friend_code)));

    SUSC_REQUIRE(hazel_message_reader_string(
        reader, no->puid, sizeof(no->puid)));

    // TODO GameData stuff
    if (initial_state)
    {

    }

    return 0;
}

int susc_no_player_info_serialize(
	susc_net_object *net_object, susc_game_state *state, 
    hazel_message_writer *writer, bool initial_state)
{
    printf("!!!!! Player info serialize\n");
    susc_no_player_info *no = (susc_no_player_info *)net_object;

    SUSC_REQUIRE(hazel_message_writer_uint8(writer, no->player_id));
    SUSC_REQUIRE(hazel_message_writer_packed_int32(writer, no->client_id));

    for (int i = 0; i < SUSC_PLAYER_OUTFIT_COUNT; i++)
    {
        if (no->outfits[i] != NULL)
        {
            SUSC_REQUIRE(hazel_message_writer_uint8(
                writer, (uint8_t)i));
            SUSC_REQUIRE(susc_no_player_info_outfit_serialize(
                no->outfits[i], writer));
        }
    }

    SUSC_REQUIRE(hazel_message_writer_packed_uint32(writer, no->player_level));

    uint8_t flags = 0;
    if (no->disconnected)
    {
        flags |= 1;
    }
    if (no->is_dead)
    {
        flags |= 4;
    }
    SUSC_REQUIRE(hazel_message_writer_uint8(writer, flags));

    // TODO Roles
    SUSC_REQUIRE(hazel_message_writer_uint16(
        writer, (uint16_t)no->role_type));
    if (no->role_when_alive != SUSC_ROLE_TYPES_COUNT)
    {
        SUSC_REQUIRE(hazel_message_writer_uint16(
            writer, (uint16_t)no->role_when_alive));
    }

    uint8_t task_count = 0;
    struct susc_no_player_info_task_info_list *task_iter = no->tasks;
    while (task_iter != NULL)
    {
        task_count++;
        task_iter = task_iter->next;
    }

    task_iter = no->tasks;
    while (task_iter != NULL)
    {
        SUSC_REQUIRE(susc_no_player_info_task_info_serialize(
            &task_iter->task, writer));
        task_iter = task_iter->next;
    }

    SUSC_REQUIRE(hazel_message_writer_string(writer, no->friend_code));
    SUSC_REQUIRE(hazel_message_writer_string(writer, no->puid));

    if (!initial_state)
    {
        no->base.dirty_bits = 0;
    }

    return 0;
}

int susc_no_player_info_handle_rpc(
	susc_net_object *net_object, susc_game_state *game_state,
    uint8_t call_id, hazel_message_reader *message_reader)
{
    // TODO
    return 0;
}

int susc_no_player_info_on_spawn(
    susc_net_object *net_object, susc_game_state *game_state)
{
    // TODO
    return 0;
}
