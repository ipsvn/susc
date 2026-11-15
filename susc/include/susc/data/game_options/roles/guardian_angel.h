#pragma once

#include "susc/common.h"

typedef struct hazel_message_reader hazel_message_reader;
typedef struct hazel_message_writer hazel_message_writer;

typedef struct susc_role_options_guardian_angel
{
    uint8_t cooldown;
    uint8_t protection_duration;
    bool impostors_can_see_protect;
} susc_role_options_guardian_angel;

int susc_role_options_guardian_angel_serialize(
    susc_role_options_guardian_angel *guardian_angel,
    hazel_message_writer *writer);
int susc_role_options_guardian_angel_deserialize(
    susc_role_options_guardian_angel *guardian_angel,
    hazel_message_reader *reader);
