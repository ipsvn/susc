#pragma once

#include "susc/common.h"
#include "susc/game/state.h"

typedef struct susc_net_object susc_net_object;

typedef struct susc_net_object_container
{
    uint32_t net_id;
    susc_net_object *obj;
    struct susc_net_object_container *next;
} susc_net_object_container;


void susc_state_no_init(susc_game_state *state);
int susc_state_no_insert_netid(
    susc_game_state *state, uint32_t net_id, susc_net_object *no);
int susc_state_no_insert(
    susc_game_state *state, susc_net_object *no);
susc_net_object_container *susc_state_no_find_container(
    susc_game_state *state, uint32_t net_id);
susc_net_object *susc_state_no_find(
    susc_game_state *state, uint32_t net_id);
bool susc_state_no_replace(
    susc_game_state *state, uint32_t net_id, 
    susc_net_object *new_value, susc_net_object **out_old_value);
