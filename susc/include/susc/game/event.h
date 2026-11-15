#pragma once

#include "susc/common.h"
#include "susc/errors.h"

enum susc_events
{
    SUSC_EVENT_NONE,
        
    SUSC_EVENT_JOINED_GAME,
    
    SUSC_EVENT_CLIENT_CREATED,
    SUSC_EVENT_PLAYER_SPAWNED,

    SUSC_EVENT_PLAYER_EVENT,
    SUSC_EVENT_PLAYER_TRANSFORM_EVENT,

    SUSC_EVENT_MAX
};

typedef struct susc_event_base
{
    enum susc_events type;
    
} susc_event_base;

typedef struct susc_event_queue_item
{
    struct susc_event_queue_item *next;
    
    susc_event_base *event;
} susc_event_queue_item;

typedef struct susc_event_queue
{
    susc_event_queue_item *head;
    susc_event_queue_item *tail;
} susc_event_queue;

susc_event_base *susc_event_malloc(
    size_t size, enum susc_events event_type);

#define SUSC_CREATE_EVENT_OF_TYPE(structname, typeenum) \
    (structname *)susc_event_malloc(sizeof(structname), typeenum)

int susc_event_queue_init(susc_event_queue *queue);
int susc_event_queue_push_event(
    susc_event_queue *queue, susc_event_base *event);
int susc_event_queue_pop(
    susc_event_queue *queue, susc_event_base **out);
int susc_event_free(
    susc_event_base *event);