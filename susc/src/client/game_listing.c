#include "susc/client/game_listing.h"

#include "../utils/utils.h"
#include "../utils/hazel_utils.h"

int susc_game_listing_deserialize(susc_game_listing *game_listing,
                                  hazel_message_reader *reader)
{
    SUSC_REQUIRE(hazel_message_reader_uint32(reader, &game_listing->ip));
    SUSC_REQUIRE(hazel_message_reader_uint16(reader, &game_listing->port));
    SUSC_REQUIRE(hazel_message_reader_int32(reader, &game_listing->game_id));
    SUSC_REQUIRE(hazel_message_reader_string(
        reader, game_listing->host_name, 
        sizeof(game_listing->host_name)));
    SUSC_REQUIRE(hazel_message_reader_uint8(reader, &game_listing->player_count));
    SUSC_REQUIRE(hazel_message_reader_packed_int32(reader, &game_listing->age));
    SUSC_REQUIRE(hazel_message_reader_uint8(reader, &game_listing->map_id));
    SUSC_READER_TYPECONV_REQUIRE(reader, uint8, game_listing->num_impostors);
    SUSC_READER_TYPECONV_REQUIRE(reader, uint8, game_listing->max_players);
    SUSC_READER_TYPECONV_REQUIRE(reader, uint8, game_listing->platform);
    SUSC_REQUIRE(hazel_message_reader_string(
        reader, game_listing->host_platform_name,
        sizeof(game_listing->host_platform_name)));
    SUSC_REQUIRE(hazel_message_reader_uint32(reader, &game_listing->language));
    return 0;
}
