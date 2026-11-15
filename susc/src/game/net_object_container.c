#include "net_object_container.h"
#include "susc/game/state.h"

#include "susc/errors.h"
#include "susc/data/net_object.h"

#include <stdlib.h>

void susc_state_no_init(susc_game_state *state)
{
    for (size_t i = 0; i < SUSC_NET_OBJECT_CONTAINER_BUCKETS; i++)
    {
        state->net_objects[i] = NULL;
    }
}
int susc_state_no_insert_netid(
    susc_game_state *state, uint32_t net_id, susc_net_object *no)
{
    size_t n = net_id % SUSC_NET_OBJECT_CONTAINER_BUCKETS;
    susc_net_object_container *container = 
        malloc(sizeof(susc_net_object_container));
    if (container == NULL)
    {
        return SUSC_ERR_FAILED_ALLOC;
    }

    container->net_id = net_id;
    container->obj = no;
    container->next = state->net_objects[n];
    state->net_objects[n] = container;
    return 0;
}
int susc_state_no_insert(
    susc_game_state *state, susc_net_object *no)
{    
    return susc_state_no_insert_netid(state, no->net_id, no);
}
susc_net_object_container *susc_state_no_find_container(
    susc_game_state *state, uint32_t net_id)
{
    size_t n = net_id % SUSC_NET_OBJECT_CONTAINER_BUCKETS;
    susc_net_object_container *entry = state->net_objects[n];
    while (entry != NULL) {
        if (entry->net_id == net_id) {
            return entry;
        }
        entry = entry->next;
    }
    return NULL;
}
susc_net_object *susc_state_no_find(
    susc_game_state *state, uint32_t net_id)
{
    susc_net_object_container *container =
        susc_state_no_find_container(state, net_id);
    if (container == NULL)
    {
        return NULL;
    }
    return container->obj;
}
bool susc_state_no_replace(
    susc_game_state *state, uint32_t net_id, 
    susc_net_object *new_value, susc_net_object **out_old_value)
{
    susc_net_object_container *container = 
        susc_state_no_find_container(state, net_id);
    if (container == NULL)
    {
        return SUSC_ERR_FAILED_TODOERR;
    }
    if (out_old_value != NULL)
    {
        *out_old_value = container->obj;
    }
    container->obj = new_value;
    return 0;
}
