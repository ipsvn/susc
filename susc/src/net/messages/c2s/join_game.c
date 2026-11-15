#include "susc/net/messages/c2s/join_game.h"

#include "susc/net/messages/tags.h"

#include "../../../utils/hazel_utils.h"

int susc_c2s_join_game_serialize(hazel_message_writer *writer,
                                 int32_t game_code, bool enable_crossplay)
{
    SUSC_REQUIRE(hazel_message_writer_start_message(writer, SUSC_MESSAGE_TAG_JOINGAME));
    SUSC_REQUIRE(hazel_message_writer_int32(writer, game_code));
    SUSC_REQUIRE(hazel_message_writer_bool(writer, !enable_crossplay));
    SUSC_REQUIRE(hazel_message_writer_end_message(writer));
    return 0;
}
