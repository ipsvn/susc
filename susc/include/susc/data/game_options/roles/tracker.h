#pragma once

#include "susc/common.h"

typedef struct hazel_message_reader hazel_message_reader;
typedef struct hazel_message_writer hazel_message_writer;

typedef struct susc_role_options_tracker
{
    uint8_t cooldown;
    uint8_t duration;
    uint8_t delay;
} susc_role_options_tracker;

int susc_role_options_tracker_serialize(
    susc_role_options_tracker *tracker,
    hazel_message_writer *writer);
int susc_role_options_tracker_deserialize(
    susc_role_options_tracker *tracker,
    hazel_message_reader *reader);
