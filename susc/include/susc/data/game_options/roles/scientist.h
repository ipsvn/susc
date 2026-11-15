#pragma once

#include "susc/common.h"

typedef struct hazel_message_reader hazel_message_reader;
typedef struct hazel_message_writer hazel_message_writer;

typedef struct susc_role_options_scientist
{
    uint8_t cooldown;
    uint8_t battery_charge;
} susc_role_options_scientist;

int susc_role_options_scientist_serialize(
    susc_role_options_scientist *scientist,
    hazel_message_writer *writer);
int susc_role_options_scientist_deserialize(
    susc_role_options_scientist *scientist,
    hazel_message_reader *reader);
