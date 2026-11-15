#include "susc/game/event.h"

#include <malloc.h>

susc_event_base *susc_event_malloc(
    size_t size, enum susc_events event_type)
{
    susc_event_base *event = malloc(size);
    if (event == NULL)
    {
        return NULL;
    }
    event->type = event_type;
    return event;
}

int susc_event_queue_init(susc_event_queue *queue)
{
    queue->head = NULL;
    queue->tail = NULL;
    return 0;
}

int susc_event_queue_push_event(
    susc_event_queue *queue, susc_event_base *event)
{
    SUSC_LOG_DEBUG("Push event of type %d", event->type);
    susc_event_queue_item *old_tail = queue->tail;
    
    susc_event_queue_item *new_item = malloc(sizeof(susc_event_queue_item));
    if (new_item == NULL)
    {
        return SUSC_ERR_FAILED_ALLOC;
    }
    
    new_item->event = event;
    new_item->next = NULL;

    if (old_tail != NULL)
    {
        old_tail->next = new_item;
    }

    queue->tail = new_item;
    if (queue->head == NULL)
    {
        queue->head = new_item;
    }

    return 0;
}

int susc_event_queue_pop(
    susc_event_queue *queue, susc_event_base **out)
{
    if (queue->head == NULL)
    {
        *out = NULL;
        return 1;
    }

    susc_event_queue_item *item = queue->head;
    *out = item->event;
    queue->head = item->next;

    if (queue->tail == item)
    {
        queue->tail = NULL;
    }

    free(item);

    return 0;
}

int susc_event_free(
    susc_event_base* event)
{
    free(event);
    return 0;
}