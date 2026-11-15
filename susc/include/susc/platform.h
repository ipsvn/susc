#pragma once
#include "common.h"

#include "hazel/writer.h"
#include "hazel/reader.h"

enum susc_platform_type {
    SUSC_PLATFORM_UNKNOWN     = 0,
    SUSC_PLATFORM_EPIC        = 1,
    SUSC_PLATFORM_STEAM       = 2,
    SUSC_PLATFORM_MAC         = 3,
    SUSC_PLATFORM_WIN10       = 4,
    SUSC_PLATFORM_ITCHIO      = 5,
    SUSC_PLATFORM_IPHONE      = 6,
    SUSC_PLATFORM_ANDROID     = 7,
    SUSC_PLATFORM_SWITCH      = 8,
    SUSC_PLATFORM_XBOX        = 9,
    SUSC_PLATFORM_PLAYSTATION = 10
};

typedef struct susc_platform {
    enum susc_platform_type type;
    char name[16];
    uint64_t platform_id;
} susc_platform;

int susc_platform_serialize(susc_platform *platform, 
                             hazel_message_writer *writer);
int susc_platform_deserialize(susc_platform *platform, 
                               hazel_message_reader *reader);
