#pragma once

#include "susc/common.h"
#include "susc/platform.h"

typedef struct hazel_message_reader hazel_message_reader;

typedef struct susc_game_listing
{
    uint32_t ip;
    uint16_t port;
    int32_t game_id;
    uint8_t player_count;
    char host_name[SUSC_MAX_PLAYER_NAME_LENGTH + 1];
    char host_platform_name[16];
    enum susc_platform_type platform;
    int32_t age;
    int32_t max_players;
    int32_t num_impostors;
    uint8_t map_id;
    uint32_t language;
} susc_game_listing;

int susc_game_listing_deserialize(susc_game_listing *game_listing,
                                  hazel_message_reader *reader);
