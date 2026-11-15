#pragma once

#include "susc/common.h"

typedef struct hazel_message_reader hazel_message_reader;
typedef struct hazel_message_writer hazel_message_writer;

typedef struct susc_role_rate
{
    uint8_t max_count;
    uint8_t chance;
} susc_role_rate;

int susc_role_rate_serialize(susc_role_rate *role_rate, 
                             hazel_message_writer *writer);

int susc_role_rate_deserialize(susc_role_rate *role_rate, 
                               hazel_message_reader *reader);
