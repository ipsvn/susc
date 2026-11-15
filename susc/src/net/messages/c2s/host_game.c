#include "susc/net/messages/c2s/host_game.h"

#include "susc/net/messages/tags.h"

#include "../../../utils/hazel_utils.h"

int susc_c2s_host_game_serialize(
    hazel_message_writer *writer,
    susc_game_options *game_options, bool enable_crossplay)
{
    SUSC_REQUIRE(hazel_message_writer_start_message(writer, SUSC_MESSAGE_TAG_HOSTGAME));

    {
        uint8_t buffer[128];
        hazel_message_writer options_writer;
        SUSC_REQUIRE(hazel_message_writer_init(
            &options_writer, buffer, sizeof(buffer)));
        
        SUSC_REQUIRE(susc_game_options_serialize(game_options, &options_writer));
        SUSC_REQUIRE(hazel_message_writer_bytes_and_size(
            writer, buffer, 0, options_writer.size));
    }

    SUSC_REQUIRE(hazel_message_writer_int32(writer, enable_crossplay ? INT32_MAX : 4));

    SUSC_REQUIRE(hazel_message_writer_end_message(writer));
    return 0;
}
