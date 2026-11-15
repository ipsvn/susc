#pragma once

#include "susc/common.h"

typedef struct hazel_message_reader hazel_message_reader;
typedef struct hazel_message_writer hazel_message_writer;

typedef struct susc_role_options_noisemaker
{
    bool impostor_alert;
    uint8_t alert_duration;
} susc_role_options_noisemaker;

int susc_role_options_noisemaker_serialize(
    susc_role_options_noisemaker *noisemaker,
    hazel_message_writer *writer);
int susc_role_options_noisemaker_deserialize(
    susc_role_options_noisemaker *noisemaker,
    hazel_message_reader *reader);
