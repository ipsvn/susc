#pragma once
#include "../common.h"

#include "auth.h"
#include "game_listing.h"
#include "language.h"
#include "version.h"
#include "../chat_mode.h"

#include "susc/gamecode.h"
#include "susc/data/game_options/game_options.h"
#include "susc/game/event.h"
#include "susc/game/state.h"
#include "hazel/udp/client.h"
#include "hazel/ip_mode.h"

/** \defgroup client Among Us client
 *
 *  \brief Implementation of an Among us client
 *  @{
 */

typedef struct susc_client
{
    susc_game_state state;
    hazel_udp_client udp_client;

    susc_version version;
    susc_auth_data auth_data;
    
    enum susc_client_language language;
    enum susc_chat_mode chat_mode;
} susc_client;

int susc_client_init(susc_client *client,
                     const char *player_name,
                     susc_platform *platform,
                     susc_version version,
                     const susc_auth_data *auth_data);

/**
 * \brief Connect to an Among Us game server.
 * This function will block until the full handshake is done.
 */
int susc_client_connect(susc_client *client, 
                        const char *hostname, uint16_t port,
                        enum hazel_ip_mode ip_mode);

/**
 * \brief Disconnect from the Among Us game server.
 */
int susc_client_disconnect(susc_client *client);

/**
 * \brief Host a game on the connected game server.
 * This function will not block, as it simply sends the message requesting to
 * host a game.
 *
 * \param client client
 */
int susc_client_host_game(susc_client *client, susc_game_options *options);

/**
 * \brief Join a game on the connected game server.
 * This function will not block, as it simply sends the message requesting to
 * join the game.
 * 
 * \param client    client
 * \param game_code The game code to join.
 * 
 * \return 0 on success
 */
int susc_client_join_game(susc_client *client, int32_t game_code);


int susc_client_loop(susc_client *client, susc_event_base **event);

int susc_client_write_hello_data(susc_client *client, 
                                 hazel_message_writer *writer);

int susc_client_send_message(susc_client *client, hazel_message_writer *writer);
