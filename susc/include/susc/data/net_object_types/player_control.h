#pragma once

#include "susc/data/net_object.h"

#include "susc/data/player_outfit_type.h"
#include "susc/data/enums/player_color.h"

#include "hazel/reader.h"
#include "hazel/writer.h"

/**
 * The string ID for no pet, use with #susc_no_player_control_rpc_set_pet
 */
#define SUSC_PC_PET_EMPTY_ID "pet_EmptyPet"

/**
 * The string ID for no hat, use with #susc_no_player_control_rpc_set_hat
 */
#define SUSC_PC_HAT_NONE_ID "hat_NoHat"

/**
 * The string ID for no visor, use with #susc_no_player_control_rpc_set_visor
 */
#define SUSC_PC_VISOR_EMPTY_ID "visor_EmptyVisor"

/**
 * The string ID for no nameplate, use with #susc_no_player_control_rpc_set_nameplate
 */
#define SUSC_PC_NAMEPLATE_NONE_ID "nameplate_NoPlate"

typedef struct susc_no_player_physics susc_no_player_physics;
typedef struct susc_no_network_transform susc_no_network_transform;

typedef struct susc_no_player_control
{
    susc_net_object base;

    bool is_new;
    uint8_t player_id;
} susc_no_player_control;

/**
 * \brief Request the host to set the player name
 * 
 * \param name The name to use, maximum length 10 characters
 */
int susc_no_player_control_cmd_check_name(
    susc_no_player_control *obj, susc_game_state *state,
    const char *name);
/**
 * \brief Request the host to set the player color
 * 
 * \param color The color to use
 */
int susc_no_player_control_cmd_check_color(
    susc_no_player_control *obj, susc_game_state *state,
    enum susc_player_color color);

int susc_no_player_control_rpc_set_level(
    susc_no_player_control* obj, susc_game_state* state,
    uint32_t level);

int susc_no_player_control_rpc_set_pet(
    susc_no_player_control* obj, susc_game_state* state,
    char* pet_id);

int susc_no_player_control_rpc_set_hat(
    susc_no_player_control* obj, susc_game_state* state,
    char* hat_id);

int susc_no_player_control_rpc_set_skin(
    susc_no_player_control* obj, susc_game_state* state,
    char* skin_id);

int susc_no_player_control_rpc_set_visor(
    susc_no_player_control* obj, susc_game_state* state,
    char* visor_id);

int susc_no_player_control_rpc_set_nameplate(
    susc_no_player_control* obj, susc_game_state* state,
    char* visor_id);

/**
 * The length limit in game is 100 charcters, however it will work up until the
 * reader max length.
 */
int susc_no_player_control_rpc_send_chat(
    susc_no_player_control *obj, susc_game_state *state,
    const char *message);



susc_no_player_physics *susc_no_player_control_get_physics(
    susc_no_player_control *obj);
susc_no_network_transform *susc_no_player_control_get_transform(
    susc_no_player_control *obj);

int susc_no_player_control_instantiate(susc_net_object **net_object);
static struct susc_net_object_prefab susc_no_player_control_type = {
	.instantiate = susc_no_player_control_instantiate
};