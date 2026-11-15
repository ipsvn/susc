#include "susc/data/game_options/role_rate.h"

#include "../../utils/utils.h"

#include "hazel/reader.h"
#include "hazel/writer.h"

int susc_role_rate_serialize(susc_role_rate *role_rate, 
                             hazel_message_writer *writer)
{
    SUSC_REQUIRE(hazel_message_writer_uint8(writer, role_rate->max_count));
    SUSC_REQUIRE(hazel_message_writer_uint8(writer, role_rate->chance));
    return 0;
}

int susc_role_rate_deserialize(susc_role_rate *role_rate, 
                               hazel_message_reader *reader)
{
    SUSC_REQUIRE(hazel_message_reader_uint8(reader, &role_rate->max_count));
    SUSC_REQUIRE(hazel_message_reader_uint8(reader, &role_rate->chance));
    return 0;
}
