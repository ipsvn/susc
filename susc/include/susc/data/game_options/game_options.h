#pragma once

#include "susc/common.h"

#include "susc/data/game_options/role_options.h"

#include "susc/data/enums/game_keywords.h"
#include "susc/data/enums/gamemode.h"
#include "susc/data/enums/kill_distances.h"
#include "susc/data/enums/map_types.h"
#include "susc/data/enums/rules_presets.h"
#include "susc/data/enums/task_bar_update.h"

typedef struct susc_game_options_base
{
    uint8_t version;
    enum susc_gamemode gamemode;
    enum susc_special_gamemode special_mode;
    enum susc_rules_presets rules_preset;
    uint8_t max_players;
    enum susc_game_keywords game_keywords;
    enum susc_map_types map_type;
    int32_t num_impostors;
    bool is_defaults;
} susc_game_options_base;

typedef struct susc_game_options_normal
{
    float player_speed_mod;
    float crew_light_mod;
    float impostor_light_mod;
    float kill_cooldown;
    int num_common_tasks;
    int num_long_tasks;
    int num_short_tasks;
    int num_emergency_meetings;
    int emergency_cooldown;
    bool ghosts_do_tasks;
    enum susc_kill_distances kill_distance;
    int discussion_time;
    int voting_time;
    bool confirm_impostor;
    bool visual_tasks;
    bool anonymous_votes;
    enum susc_task_bar_update task_bar_update;
    susc_role_options_collection role_options;
} susc_game_options_normal;

typedef struct susc_game_options_hidenseek
{
    float player_speed_mod;
    float crew_light_mod;
    float impostor_light_mod;
    int num_common_tasks;
    int num_long_tasks;
    int num_short_tasks;
    int crewmate_vent_uses;
    float crewmate_time_in_vent;
    float hiding_time;
    float crewmate_flashlight_size;
    float impostor_flashlight_size;
    bool use_flashlight;
    bool final_hideseek_map;
    float final_hide_time;
    float final_seeker_speed;
    bool final_hide_pings;
    bool show_names;
    uint32_t seeker_player_id;
    float max_ping_time;
} susc_game_options_hidenseek;

typedef struct susc_game_options
{
    susc_game_options_base base_options;
    union
    {
        susc_game_options_normal normal;
        susc_game_options_hidenseek hidenseek;
    } mode;
} susc_game_options;

int susc_game_options_serialize(susc_game_options *game_options, 
                                hazel_message_writer *writer);

int susc_game_options_deserialize(susc_game_options *game_options, 
                                  hazel_message_reader *reader);
