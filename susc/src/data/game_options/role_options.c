#include "susc/data/game_options/role_options.h"

#include "../../utils/hazel_utils.h"

int susc_role_options_collection_serialize(
    susc_role_options_collection *roles,
    hazel_message_writer *writer)
{
    int32_t role_data_count = 0;
    for (size_t i = 0; i < SUSC_ROLE_TYPES_COUNT; i++)
    {
        if (roles->role_data[i].has_data)
        {
            role_data_count++;
        }
    }
    SUSC_REQUIRE(hazel_message_writer_packed_int32(
        writer, role_data_count));

    for (int16_t i = 0; i < SUSC_ROLE_TYPES_COUNT; i++)
    {
        susc_role_data data = roles->role_data[i];
        if (!data.has_data)
        {
            continue;
        }
        SUSC_REQUIRE(hazel_message_writer_int16(writer, i));
        SUSC_REQUIRE(susc_role_rate_serialize(&data.role_rate, writer));
    
        SUSC_REQUIRE(hazel_message_writer_start_message(writer, 0));

        SUSC_REQUIRE(hazel_message_writer_end_message(writer));


    }

    return 0;
}
