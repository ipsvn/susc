#include "susc/platform.h"

#include "utils/utils.h"

int susc_platform_serialize(susc_platform *platform, 
                            hazel_message_writer *writer)
{
    int ret = 0;
    SUSC_REQUIRE(hazel_message_writer_start_message(writer, platform->type));
    SUSC_REQUIRE(hazel_message_writer_string(writer, platform->name));
    switch (platform->type)
    {
        case SUSC_PLATFORM_WIN10:
        case SUSC_PLATFORM_XBOX:
        case SUSC_PLATFORM_PLAYSTATION:
            SUSC_REQUIRE(hazel_message_writer_uint64(writer, platform->platform_id));
            break;
        default:
            break;
    }
    SUSC_REQUIRE(hazel_message_writer_end_message(writer));
    return ret;
}

int susc_platform_deserialize(susc_platform *platform, 
                               hazel_message_reader *reader)
{
    platform->type = reader->tag;
    SUSC_REQUIRE(hazel_message_reader_string(
        reader, platform->name, sizeof(platform->name)));
    switch (platform->type)
    {
        case SUSC_PLATFORM_WIN10:
        case SUSC_PLATFORM_XBOX:
        case SUSC_PLATFORM_PLAYSTATION:
            SUSC_REQUIRE(hazel_message_reader_uint64(reader, &platform->platform_id));
            break;
        default:
            break;
    }
    return 0;
}
