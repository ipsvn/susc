#pragma once

#include "susc/data/net_object.h"

#include "susc/data/enums/role_types.h"
#include "susc/data/player_outfit_type.h"

#include "hazel/reader.h"
#include "hazel/writer.h"

typedef struct susc_no_player_info_task_info
{
    uint32_t id;
    uint8_t type_id;
    bool complete;
} susc_no_player_info_task_info;
struct susc_no_player_info_task_info_list;

int susc_no_player_info_task_info_serialize(
    susc_no_player_info_task_info *task_info,
    hazel_message_writer *writer);
int susc_no_player_info_task_info_deserialize(
    susc_no_player_info_task_info *task_info,
    hazel_message_reader *reader);

typedef struct susc_no_player_info_outfit
{
    int32_t color_id;
    char* hat_id;
    char* pet_id;
    char* skin_id;
    char* visor_id;
    char* name_plate_id;
    char* player_name;
    uint8_t hat_sequence_id;
    uint8_t pet_sequence_id;
    uint8_t skin_sequence_id;
    uint8_t visor_sequence_id;
    uint8_t name_plate_sequence_id;
} susc_no_player_info_outfit;

int susc_no_player_info_outfit_serialize(
    susc_no_player_info_outfit *outfit,
    hazel_message_writer *writer);
int susc_no_player_info_outfit_deserialize(
    susc_no_player_info_outfit *outfit,
    hazel_message_reader *reader);


typedef struct susc_no_player_info
{
    susc_net_object base;
    susc_no_player_info_outfit *outfits[SUSC_PLAYER_OUTFIT_COUNT];
    struct susc_no_player_info_task_info_list *tasks;

    uint8_t player_id;
    int32_t client_id;

    uint32_t player_level;
    bool disconnected;
    bool is_dead;
    enum susc_role_types role_type;
    enum susc_role_types role_when_alive;

    char friend_code[64 + 1]; // TODO this length is just randomly decided
    char puid[SUSC_EOS_PRODUCTUSERID_MAX_LENGTH + 1];

} susc_no_player_info;

int susc_no_player_info_deserialize(
	susc_net_object *net_object, susc_game_state *game_state, 
    hazel_message_reader *message_reader, bool initial_state);

int susc_no_player_info_serialize(
	susc_net_object *net_object, susc_game_state *game_state, 
    hazel_message_writer *message_writer, bool initial_state);

int susc_no_player_info_handle_rpc(
	susc_net_object *net_object, susc_game_state *game_state,
    uint8_t call_id, hazel_message_reader *message_reader);

int susc_no_player_info_on_spawn(
    susc_net_object *net_object, susc_game_state *game_state);


int susc_no_player_info_instantiate(susc_net_object **net_object);
static struct susc_net_object_prefab susc_no_player_info_type = {
	.instantiate = susc_no_player_info_instantiate
};