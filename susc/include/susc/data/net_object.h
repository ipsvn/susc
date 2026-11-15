#pragma once

#include "susc/common.h"

#include "spawn_flags.h"
#include "spawn_type.h"

#include "hazel/send_option.h"

#define SUSC_NET_OBJECT_NOT_SERIALIZABLE -0x5401

enum susc_net_object_type
{
    SUSC_NET_OBJECT_TYPE_UNKNOWN,
    SUSC_NET_OBJECT_TYPE_PLAYER_CONTROL,
    SUSC_NET_OBJECT_TYPE_LOBBY_BEHAVIOUR,
    SUSC_NET_OBJECT_TYPE_PLAYER_INFO,
    SUSC_NET_OBJECT_TYPE_NETWORK_TRANSFORM,
    SUSC_NET_OBJECT_TYPE_PLAYER_PHYSICS,
    SUSC_NET_OBJECT_TYPE_MAX
};

typedef struct hazel_message_reader hazel_message_reader;
typedef struct hazel_message_writer hazel_message_writer;
typedef struct susc_game_state susc_game_state;
typedef struct susc_net_object susc_net_object;

typedef int (*susc_net_object_deserialize)(
    susc_net_object *net_object, susc_game_state *game_state, 
    hazel_message_reader *message_reader, bool initial_state);
typedef int (*susc_net_object_serialize)(
    susc_net_object *net_object, susc_game_state *game_state,
    hazel_message_writer *message_writer, bool initial_state);
typedef int (*susc_net_object_handle_rpc)(
    susc_net_object *net_object, susc_game_state *game_state,
    uint8_t call_id, hazel_message_reader *message_reader);
typedef int (*susc_net_object_on_spawn)(
    susc_net_object *net_object, susc_game_state *game_state);

typedef int (*susc_net_object_destroy)(
    susc_net_object *net_object);

typedef struct susc_net_object
{
    // virtual functions
    susc_net_object_deserialize deserialize;
    susc_net_object_serialize serialize;
    susc_net_object_handle_rpc handle_rpc;
    susc_net_object_on_spawn on_spawn;

    susc_net_object_destroy destroy;

    enum susc_spawn_type spawn_type;
    uint32_t net_id;
    uint32_t dirty_bits;
    enum susc_spawn_flags spawn_flags;
    enum hazel_send_option send_option;
    int32_t owner_id;

    enum susc_net_object_type _type;
    struct susc_net_object *_child;
    struct susc_net_object *_first;
    struct susc_net_object *_next;

} susc_net_object;

#define SUSC_NO_ITERATE_CHILDREN(obj, varname)        \
    for (susc_net_object *susc_no_ic = (obj)->_first; \
         susc_no_ic != NULL;                          \
         susc_no_ic = susc_no_ic->_child)             \
    for (susc_net_object *varname = susc_no_ic;       \
         varname != NULL;                             \
         varname = varname->_next)

int susc_net_object_children_count(susc_net_object *net_object);

susc_net_object *susc_net_object_find_child_of_type(
    susc_net_object *net_object, 
    enum susc_net_object_type type);

int susc_net_object_default_destroy(susc_net_object *net_object);

struct susc_net_object_prefab {
	int	(*instantiate)(susc_net_object **net_object);
};
