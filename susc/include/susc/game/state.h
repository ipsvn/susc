#pragma once

#include "susc/common.h"
#include "event.h"
#include "../platform.h"

#include "susc/game/client_data.h"
#include "susc/game/client_list.h"

#include "susc/data/net_object.h"

/** \defgroup state Game State
 *
 *  \brief Base implementation keeping track of game state.
 *  You likely want to use the \ref client if you want to connect to a real 
 *  server.
 *  @{
 */

/** 
 * Tried to spawn a net object when not the host of the game 
 * This is disallowed, and would've otherwise probably caused an anticheat kick
 */
#define SUSC_ERR_STATE_SPAWN_NOT_HOST -0x5001
/** gamedata data was received, but the associated net object was not found */
#define SUSC_ERR_STATE_GAMEDATA_MISSING_NO -0x5002
/** A spawn request was received with an unknown/out of range prefab ID. */
#define SUSC_ERR_STATE_SPAWN_UNKNOWN_PREFAB -0x5003
/** 
 * A spawn request was received, but the spawned prefab component count and
 * encoded component count were mismatched.
 */
#define SUSC_ERR_STATE_SPAWN_MISMATCHED_COMPONENTS -0x5004

/**
 * Number of buckets to use in the net object table.
 */
#ifndef SUSC_NET_OBJECT_CONTAINER_BUCKETS
#define SUSC_NET_OBJECT_CONTAINER_BUCKETS 128
#endif

typedef struct susc_net_object_container susc_net_object_container;

typedef struct susc_game_state
{
    void (*handle_send)(struct susc_game_state *state, 
                        hazel_message_writer *writer);

    susc_event_queue event_queue;

    char player_name[SUSC_MAX_PLAYER_NAME_LENGTH + 1];
    susc_platform platform;
    int32_t client_id;
    int32_t host_id;
    int32_t game_id;

    struct susc_state_client_list *client_list;
    susc_client_data *my_client;

    susc_net_object_container *net_objects[SUSC_NET_OBJECT_CONTAINER_BUCKETS];
    uint32_t _net_id_count;

} susc_game_state;

/**
 * \brief Initialize the susc_game_state structure
 */
int susc_state_init(susc_game_state *state,
                    const char* player_name,
                    susc_platform *platform);

/**
 * \brief Add a client with the data stored in \param data
 * 
 * This function copies the client struct passed in as \param data into an 
 * internal client list and returns the pointer to the client in that list.
 * 
 * \return The pointer to the new client
 */
susc_client_data *susc_state_create_client(susc_game_state *state,
                                           susc_client_data data);

/**
 * \brief Find a client in the game
 * 
 * \param state state
 * \param id    The client id to search for 
 *
 * \return \c NULL if no matching client id is in this game
 * \return A susc_client_data pointer of the client if found
 */
susc_client_data *susc_state_find_client(susc_game_state *state, int32_t id);

/**
 * \brief Get my client in the game
 * 
 * \param state state
 *
 * \return \c NULL if not joined to a game yet
 * \return A susc_client_data pointer of the client if found
 */
susc_client_data *susc_state_my_client(susc_game_state *state);

susc_client_data *susc_state_find_or_create_client(susc_game_state *state,
                                                   susc_client_data *data);

/**
 * \brief Spawn a net object into the game.
 * 
 * You should only call this function if this client is the host of the game,
 * otherwise the function will return an error (TODO override this if wanted).
 * The game expects only the host to spawn objects, so this will likely cause an
 * anticheat kick if attempted. 
 * 
 * \param state    The susc_game_state to spawn into
 * \param obj      The net object to spawn.
 * \param owner_id The owner of the net object to spawn.
 * \param flags    The flags to use when spawning the object.
 * 
 * \return #SUSC_ERR_STATE_SPAWN_NOT_HOST if the client is not the host 
 * \return 0 on success
 */
int susc_state_spawn(susc_game_state *state, susc_net_object *obj,
                     int owner_id, enum susc_spawn_flags flags);

/**
 * \brief Despawn a net object from the game
 * This should only be called on objects owned by the player or if the susc
 * client is the host.
 */
int susc_state_despawn(susc_game_state *state, susc_net_object *obj);

int susc_state_gamedata_start(
    susc_game_state *state, hazel_message_writer *writer);
int susc_state_gamedata_start_to(
    susc_game_state *state, hazel_message_writer *writer,
    uint8_t call_id);
int susc_state_gamedata_end(
    susc_game_state *state, hazel_message_writer *writer);

int susc_state_rpc_start(
    susc_game_state *state, hazel_message_writer *writer,
    int32_t net_id,
    uint8_t call_id);
int susc_state_rpc_start_to(
    susc_game_state *state, hazel_message_writer *writer,
    int32_t net_id, int32_t client_id,
    uint8_t call_id);
int susc_state_rpc_end(
    susc_game_state *state, hazel_message_writer *writer);

/** @}*/

int susc_state_handle_message(
    susc_game_state *state, hazel_message_reader *reader);

int susc_state_handle_gamedata_message(
    susc_game_state *state, hazel_message_reader *reader);
