#pragma once

#include "susc/common.h"

#include "susc/platform.h"

#include "susc/data/net_object_types/player_control.h"

typedef struct susc_client_data
{
    int32_t id;
    bool in_scene;
    bool is_ready; 
    bool has_been_reported; 
    bool is_being_created;
    susc_no_player_control *character;
    uint32_t player_level;
    susc_platform platform;
    char player_name[SUSC_MAX_PLAYER_NAME_LENGTH + 1];
    // ProductUserId and FriendCode
    int32_t color_id;
} susc_client_data;
