#pragma once

#include "susc/common.h"

typedef struct hazel_message_reader hazel_message_reader;
typedef struct hazel_message_writer hazel_message_writer;

typedef struct susc_role_options_shapeshifter
{
    bool leave_skin;
    uint8_t cooldown;
    uint8_t duration;
} susc_role_options_shapeshifter;

int susc_role_options_shapeshifter_serialize(
    susc_role_options_shapeshifter *shapeshifter,
    hazel_message_writer *writer);
int susc_role_options_shapeshifter_deserialize(
    susc_role_options_shapeshifter *shapeshifter,
    hazel_message_reader *reader);
