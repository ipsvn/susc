#include "susc/game/state.h"

int susc_state_write_spawn_message(
    susc_game_state *state, susc_net_object *obj, 
    int owner_id, enum susc_spawn_flags flags,
    hazel_message_writer *writer);

int susc_state_handle_despawn_message(
    susc_game_state *state, uint32_t net_id);