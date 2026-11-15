#pragma once

#include "susc/common.h"

typedef struct hazel_message_reader hazel_message_reader;
typedef struct hazel_message_writer hazel_message_writer;

typedef struct susc_role_options_phantom
{
    uint8_t cooldown;
    uint8_t duration;
} susc_role_options_phantom;

int susc_role_options_phantom_serialize(
    susc_role_options_phantom *phantom,
    hazel_message_writer *writer);
int susc_role_options_phantom_deserialize(
    susc_role_options_phantom *phantom,
    hazel_message_reader *reader);
