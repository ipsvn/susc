#include "susc/game/state.h"

#include "susc/game/events.h"

#include "susc/errors.h"
#include "susc/data/net_object.h"
#include "susc/data/spawnable_objects.h"
#include "susc/net/messages/tags.h"

#include "gamedata_flag.h"
#include "../utils/hazel_utils.h"
#include "../utils/utils.h"

#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#include "net_object_container.h"
#include "state_internal.h"

int susc_state_init(susc_game_state *state,
                    const char* player_name,
                    susc_platform *platform)
{
    if (strlen(player_name) + 1 >= SUSC_MAX_PLAYER_NAME_LENGTH)
    {
        return 1;
    }
    
    strcpy(state->player_name, player_name);
    
    state->client_id = -1;
    state->host_id = -1;

    state->client_list = NULL;
    state->my_client = NULL;

    state->platform = *platform;

    susc_state_no_init(state);

    return 0;
}

susc_client_data *susc_state_create_client(susc_game_state *state,
                                           susc_client_data data)
{
    SUSC_LOG_DEBUG("susc_state_create_client");
    struct susc_state_client_list *client =
        malloc(sizeof(struct susc_state_client_list));
    memcpy(&client->client, &data, sizeof(susc_client_data));

    if (client->client.id == state->client_id)
    {
        state->my_client = &client->client;
    }

    client->next = NULL;

    if (state->client_list == NULL)
    {
        state->client_list = client;
        SUSC_LOG_DEBUG("client added, first client");
        return &client->client;
    }

    struct susc_state_client_list *tmp = state->client_list;
    while (tmp->next)
    {
        tmp = tmp->next;
    }
    tmp->next = client;

    SUSC_LOG_DEBUG("client added, after %d (%p)", tmp->client.id, (void *)tmp);

    susc_event_client_created *event = malloc(sizeof(susc_event_client_created));
    *event = (susc_event_client_created) {
        .base = {
            .type = SUSC_EVENT_CLIENT_CREATED
        },
        .client = &client->client
    };
    susc_event_queue_push_event(&state->event_queue, (susc_event_base *)event);

    return &client->client;
}

susc_client_data *susc_state_find_client(susc_game_state *state, int32_t id)
{
    if (id < 0)
    {
        return NULL;
    }

    struct susc_state_client_list *item = state->client_list;
    while (item != NULL)
    {
        if (item->client.id == id)
        {
            return &item->client;
        }
        item = item->next;
    }

    return NULL;
}

susc_client_data *susc_state_my_client(susc_game_state *state)
{
    return state->my_client;
}

int susc_state_spawn(susc_game_state *state, susc_net_object *obj,
                     int owner_id, enum susc_spawn_flags flags)
{
    if (state->client_id > 0 && state->host_id != state->client_id)
    {
        return SUSC_ERR_STATE_SPAWN_NOT_HOST;
    }

    if (owner_id == -3)
    {
        owner_id = state->client_id;
    }

    SUSC_NO_ITERATE_CHILDREN(obj, child)
    {
        child->owner_id = owner_id;
        child->spawn_flags = flags;
        if (child->net_id == 0)
        {
            child->net_id = state->_net_id_count++;
            susc_state_no_insert(state, obj);
        }
    }

    uint8_t writer_buf[1024];
    hazel_message_writer writer;
    SUSC_REQUIRE(hazel_message_writer_init(
        &writer, writer_buf, sizeof(writer_buf)));

    SUSC_REQUIRE(susc_state_write_spawn_message(
        state, obj, owner_id, flags, &writer));

    state->handle_send(state, &writer);

    return 0;
}

int susc_state_despawn(susc_game_state *state, susc_net_object *obj)
{
    int ret = obj->destroy(obj);
    if (ret != 0)
    {
        return ret;
    }
    susc_state_no_replace(state, obj->net_id, NULL, NULL);
    return 0;
}

int susc_state_gamedata_start(
    susc_game_state *state, hazel_message_writer *writer)
{
    SUSC_REQUIRE(hazel_message_writer_clear(writer, HAZEL_SEND_OPTION_RELIABLE));
    SUSC_REQUIRE(hazel_message_writer_start_message(writer, SUSC_MESSAGE_TAG_GAMEDATA));
    SUSC_REQUIRE(hazel_message_writer_int32(writer, state->game_id));
    return 0;
}
int susc_state_gamedata_start_to(
    susc_game_state *state, hazel_message_writer *writer,
    uint8_t client_id)
{
    SUSC_REQUIRE(hazel_message_writer_clear(writer, HAZEL_SEND_OPTION_RELIABLE));
    SUSC_REQUIRE(hazel_message_writer_start_message(writer, SUSC_MESSAGE_TAG_GAMEDATATO));
    SUSC_REQUIRE(hazel_message_writer_int32(writer, state->game_id));
    SUSC_REQUIRE(hazel_message_writer_packed_int32(writer, client_id));
    return 0;
}
int susc_state_gamedata_end(
    susc_game_state *state, hazel_message_writer *writer)
{
    SUSC_REQUIRE(hazel_message_writer_end_message(writer));
    state->handle_send(state, writer);
    return 0;
}

int susc_state_rpc_start(
    susc_game_state *state, hazel_message_writer *writer,
    int32_t net_id,
    uint8_t call_id)
{
    SUSC_REQUIRE(susc_state_gamedata_start(state, writer));
    SUSC_REQUIRE(hazel_message_writer_start_message(writer, SUSC_GAMEDATA_FLAG_RPC));
    SUSC_REQUIRE(hazel_message_writer_packed_int32(writer, net_id));
    SUSC_REQUIRE(hazel_message_writer_uint8(writer, call_id));
    return 0;
}

int susc_state_rpc_start_to(
    susc_game_state *state, hazel_message_writer *writer,
    int32_t net_id, int32_t client_id,
    uint8_t call_id)
{
    SUSC_REQUIRE(susc_state_gamedata_start_to(state, writer, client_id));
    SUSC_REQUIRE(hazel_message_writer_start_message(writer, SUSC_GAMEDATA_FLAG_RPC));
    SUSC_REQUIRE(hazel_message_writer_packed_int32(writer, net_id));
    SUSC_REQUIRE(hazel_message_writer_uint8(writer, call_id));
    return 0;
}

int susc_state_rpc_end(
    susc_game_state *state, hazel_message_writer *writer)
{
    SUSC_REQUIRE(hazel_message_writer_end_message(writer));
    SUSC_REQUIRE(susc_state_gamedata_end(state, writer));

    return 0;
}

int susc_send_scene_change(susc_game_state *state)
{
    uint8_t buffer[128];
    hazel_message_writer writer;
    SUSC_REQUIRE(hazel_message_writer_init(&writer, buffer, sizeof(buffer)));

    SUSC_REQUIRE(susc_state_gamedata_start(state, &writer));
    SUSC_REQUIRE(hazel_message_writer_start_message(
        &writer, SUSC_GAMEDATA_FLAG_SCENECHANGE));
    SUSC_REQUIRE(hazel_message_writer_packed_int32(&writer, state->client_id));
    SUSC_REQUIRE(hazel_message_writer_string(&writer, "OnlineGame"));
    SUSC_REQUIRE(hazel_message_writer_end_message(&writer));
    SUSC_REQUIRE(susc_state_gamedata_end(state, &writer));
    return 0;
}

int susc_state_handle_message(
    susc_game_state *state, hazel_message_reader *reader)
{
    switch (reader->tag)
    {
        case SUSC_MESSAGE_TAG_GAMEDATA:
        case SUSC_MESSAGE_TAG_GAMEDATATO:
        {
            int32_t msg_game_id;
            SUSC_REQUIRE(hazel_message_reader_int32(reader, &msg_game_id));

            if (reader->tag == SUSC_MESSAGE_TAG_GAMEDATATO)
            {
                int32_t intended_recipient_id;
                SUSC_REQUIRE(hazel_message_reader_packed_int32(
                    reader, &intended_recipient_id));
                if (state->client_id != intended_recipient_id)
                {
                    // TODO
                    SUSC_LOG_ERROR("gamedatato recipient wrong %d != %d",
                        intended_recipient_id, state->client_id);
                    return -1;
                }
            }

            // TODO handle whether we're in scene yet?

            while (hazel_message_reader_get_position(reader) < reader->size)
            {
                hazel_message_reader sub_reader;
                SUSC_REQUIRE(hazel_message_reader_read_message(
                    reader, &sub_reader));
                susc_state_handle_gamedata_message(
                    state, &sub_reader);
            }

            break;
        }
        case SUSC_MESSAGE_TAG_JOINEDGAME:
        {
            SUSC_LOG_DEBUG("received joined game");
            reader_print_state(reader);
            SUSC_LOG_DEBUG_PRINT_BYTES("join game", reader->data, reader->size, reader->offset);

            int32_t msg_game_id;
            SUSC_REQUIRE(hazel_message_reader_int32(reader, &msg_game_id));

            if (msg_game_id != state->game_id)
            {
                SUSC_LOG_DEBUG("Got joined game with wrong code?");
                return 0;
            }

            int32_t my_client_id;
            SUSC_REQUIRE(hazel_message_reader_int32(reader, &my_client_id));
            state->client_id = my_client_id;

            SUSC_LOG_DEBUG("my client id: %d", my_client_id);

            susc_client_data my_client = {
                .id = my_client_id,
                .platform = state->platform,
                .player_level = 0,
            };
            strcpy(my_client.player_name, 
                     state->player_name);
            if (susc_state_create_client(state, my_client) == NULL)
            {
                return SUSC_ERR_FAILED_TODOERR;
            }

            int32_t host_id;
            SUSC_REQUIRE(hazel_message_reader_int32(reader, &host_id));
            state->host_id = host_id;

            SUSC_LOG_DEBUG("host id: %d", host_id);

            int32_t client_count;
            SUSC_REQUIRE(hazel_message_reader_packed_int32(reader, &client_count));

            SUSC_LOG_DEBUG("client count: %d", client_count);

            for (int32_t i = 0; i < client_count; i++)
            {
                susc_client_data other_client = { 0 };
             
                SUSC_REQUIRE(hazel_message_reader_packed_int32(
                    reader, &other_client.id));

                reader_print_state(reader);
                
                hazel_message_reader_string_buffer_view name_view;
                SUSC_REQUIRE(hazel_message_reader_string_view(
                    reader, &name_view));

                int32_t player_name_length = name_view.length;
                if (name_view.length > SUSC_MAX_PLAYER_NAME_LENGTH - 1)
                {
                    SUSC_LOG_DEBUG("client has name exceeding name limit");
                    player_name_length = SUSC_MAX_PLAYER_NAME_LENGTH - 1;
                }
                memcpy(other_client.player_name, 
                       name_view.buffer, 
                       player_name_length);
                other_client.player_name[player_name_length] = 0;

                hazel_message_reader platform_reader;
                SUSC_REQUIRE(hazel_message_reader_read_message(
                    reader, &platform_reader));

                SUSC_REQUIRE(susc_platform_deserialize(
                    &other_client.platform, &platform_reader));
                
                SUSC_REQUIRE(hazel_message_reader_packed_uint32(
                    reader, &other_client.player_level));

                hazel_message_reader_string_buffer_view tmp;
                hazel_message_reader_string_view(reader, &tmp);
                hazel_message_reader_string_view(reader, &tmp);

                if (susc_state_create_client(state, 
                                             other_client) == NULL)
                {
                    SUSC_LOG_DEBUG("failed to create client %d", 
                                   other_client.id);
                    return SUSC_ERR_FAILED_TODOERR;
                }
            }

            printf("reached this code\n");

            SUSC_REQUIRE(susc_send_scene_change(state));

            susc_event_joined_game* event = malloc(sizeof(susc_event_joined_game));
            event->base.type = SUSC_EVENT_JOINED_GAME;
            susc_event_queue_push_event(&state->event_queue, (susc_event_base*)event);

            break;
        }
    }

    return 0;
}

int susc_state_handle_gamedata_message(
    susc_game_state *state, hazel_message_reader *reader)
{
    SUSC_LOG_DEBUG("handle gamedata %d", reader->tag);
    switch (reader->tag)
    {
        case SUSC_GAMEDATA_FLAG_DATA:
        {
            uint32_t net_id;
            SUSC_REQUIRE(hazel_message_reader_packed_uint32(reader, &net_id));
            
            susc_net_object *obj = susc_state_no_find(state, net_id);
            if (obj == NULL)
            {
                SUSC_LOG_ERROR("received data for unknown net object %d",
                               net_id);
                return SUSC_ERR_STATE_GAMEDATA_MISSING_NO;
            }

            SUSC_LOG_DEBUG("data deserialize for %d", net_id);

            return obj->deserialize(obj, state, reader, false);
        }
        case SUSC_GAMEDATA_FLAG_RPC:
        {
            uint32_t net_id;
            SUSC_REQUIRE(hazel_message_reader_packed_uint32(reader, &net_id));
            
            susc_net_object *obj = susc_state_no_find(state, net_id);
            if (obj == NULL)
            {
                SUSC_LOG_ERROR("received rpc for unknown net object %d",
                               net_id);
                return SUSC_ERR_STATE_GAMEDATA_MISSING_NO;
            }

            uint8_t call_id;
            SUSC_REQUIRE(hazel_message_reader_uint8(reader, &call_id));

            return obj->handle_rpc(obj, state, call_id, reader);
        }
        case SUSC_GAMEDATA_FLAG_SPAWN:
        {
            uint32_t spawn_id;
            SUSC_REQUIRE(hazel_message_reader_packed_uint32(reader, &spawn_id));
            
            if (spawn_id >= ARRAY_LENGTH(susc_default_spawnable_no)
                || susc_default_spawnable_no[spawn_id] == NULL)
            {
                SUSC_LOG_ERROR("spawn for unknown prefab %d", spawn_id);
                return SUSC_ERR_STATE_SPAWN_UNKNOWN_PREFAB;
            }

            int32_t owner_id;
            SUSC_REQUIRE(hazel_message_reader_packed_int32(reader, &owner_id));

            susc_client_data *owner = susc_state_find_client(state, owner_id);
            if (owner_id > 0 && owner == NULL)
            {
                SUSC_LOG_ERROR("couldn't spawn because no client");
                return SUSC_ERR_FAILED_TODOERR;
            }

            const struct susc_net_object_prefab *prefab = 
                susc_default_spawnable_no[spawn_id];

            susc_net_object *obj;
            int ret = prefab->instantiate(&obj);
            if (ret != 0)
            {
                SUSC_LOG_ERROR("Failed to spawn prefab %d", spawn_id);
                return ret;
            }

            SUSC_READER_TYPECONV_REQUIRE(reader, uint8, obj->spawn_flags);

            if (obj->spawn_flags & SUSC_SPAWN_FLAGS_CLIENT_CHARACTER)
            {
                if (prefab != &susc_no_player_control_type)
                {
                    SUSC_LOG_DEBUG("blocked attempt to set character of client to object other than player control");
                    return 0;
                }
                if (owner->character == NULL)
                {
                    owner->in_scene = true;
                    owner->character = (susc_no_player_control *) obj;

                    susc_event_player_spawned *event 
                        = malloc(sizeof(susc_event_player_spawned));
                    event->base.type = SUSC_EVENT_PLAYER_SPAWNED;
                    event->client = owner;
                    susc_event_queue_push_event(
                        &state->event_queue, (susc_event_base *)event);
                }
                else
                {
                    SUSC_LOG_DEBUG("spawned player control owner already has player control");
                    obj->destroy(obj);
                    return 0;
                }
            }

            int32_t component_count;
            SUSC_REQUIRE(hazel_message_reader_packed_int32(
                reader, &component_count));

            int32_t components_setup = 0;

            SUSC_NO_ITERATE_CHILDREN(obj, child)
            {
                SUSC_REQUIRE(hazel_message_reader_packed_uint32(
                    reader, &child->net_id));

                child->owner_id = owner_id;
                
                susc_net_object_container *obj_container =
                    susc_state_no_find_container(state, child->net_id);
                
                if (obj_container != NULL && obj_container->obj == NULL)
                {
                    // TODO destroy the obj and return;
                    obj->net_id = UINT32_MAX;
                    return 0;
                }

                SUSC_REQUIRE(susc_state_no_insert(state, child));

                hazel_message_reader component_data_msg;
                SUSC_REQUIRE(hazel_message_reader_read_message(
                    reader, &component_data_msg));

                if (component_data_msg.size > 0)
                {
                    SUSC_REQUIRE(child->deserialize(
                        child, state, &component_data_msg, true));
                }

                ++components_setup;
            }

            if (components_setup != component_count)
            {
                SUSC_LOG_ERROR(
                    "Component count mismatch for spawn of %d of type %d", 
                    obj->net_id, spawn_id);
                return SUSC_ERR_STATE_SPAWN_MISMATCHED_COMPONENTS;
            }

            SUSC_LOG_DEBUG("successfully spawned object %d of type %d with %d components",
                obj->net_id, spawn_id, component_count);

            break;
        }
        case SUSC_GAMEDATA_FLAG_DESPAWN:
        {
            uint32_t net_id;
            SUSC_REQUIRE(hazel_message_reader_packed_uint32(reader, &net_id));
            return susc_state_handle_despawn_message(state, net_id);
        }
        case SUSC_GAMEDATA_FLAG_SCENECHANGE:
        {
            int32_t client_id;
            SUSC_REQUIRE(hazel_message_reader_packed_int32(reader, &client_id));

            hazel_message_reader_string_buffer_view scene_name;
            SUSC_REQUIRE(hazel_message_reader_string_view(reader, &scene_name));

            susc_client_data *client = susc_state_find_client(state, client_id);
            if (client == NULL || scene_name.length == 0)
            {
                SUSC_LOG_DEBUG("no player with id %d in scene change",
                               client_id);
                break;
            }

            break;
        }
        default:
            HAZEL_LOG_ERROR("Unknown message tag %d in susc_state_handle_gamedata_message",
                reader->tag);
            break;
    }
    return 0;
}
