#pragma once

#include "susc/common.h"

typedef struct hazel_message_reader hazel_message_reader;
typedef struct hazel_message_writer hazel_message_writer;

typedef struct susc_role_options_engineer
{
    uint8_t cooldown;
    uint8_t in_vent_max_time;
} susc_role_options_engineer;

int susc_role_options_engineer_serialize(
    susc_role_options_engineer *engineer,
    hazel_message_writer *writer);
int susc_role_options_engineer_deserialize(
    susc_role_options_engineer *engineer,
    hazel_message_reader *reader);
