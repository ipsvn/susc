#pragma once

#include "event.h"

#include "client_data.h"
#include "susc/data/net_object_types/player_control.h"
#include "susc/utils/vector.h"

typedef struct susc_event_joined_game
{
    susc_event_base base;
} susc_event_joined_game;

typedef struct susc_event_client_created
{
    susc_event_base base;

    susc_client_data *client;
} susc_event_client_created;

typedef struct susc_event_player_spawned
{
    susc_event_base base;

    susc_client_data *client;
} susc_event_player_spawned;



enum susc_player_events
{
    SUSC_PLAYER_EVENT_RPC_SEND_CHAT,
    SUSC_PLAYER_EVENT_MAX
};
typedef struct susc_event_player_event
{
    susc_event_base base;

    enum susc_player_events player_event_type;
    susc_no_player_control *player;
} susc_event_player_event;

typedef struct susc_event_player_rpc_send_chat
{
    susc_event_player_event base;
    const char *message;
} susc_event_player_rpc_send_chat;



enum susc_player_transform_events
{
    SUSC_PLAYER_TRANSFORM_EVENT_MOVE,
    SUSC_PLAYER_TRANSFORM_EVENT_SNAP,
    SUSC_PLAYER_TRANSFORM_EVENT_MAX
};
typedef struct susc_event_player_transform_event
{
    susc_event_base base;

    enum susc_player_transform_events player_transform_event_type;
    susc_no_network_transform *transform;
} susc_event_player_transform_event;

typedef struct susc_event_player_transform_snap
{
    susc_event_player_transform_event base;

    susc_vector position;
} susc_event_player_transform_snap;

