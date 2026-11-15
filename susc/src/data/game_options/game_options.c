#include "susc/data/game_options/game_options.h"

#include "../../utils/utils.h"
#include "../../utils/hazel_utils.h"

#include "hazel/reader.h"
#include "hazel/writer.h"

int susc_game_options_normal_serialize(susc_game_options *game_options,
                                       hazel_message_writer *writer)
{
    susc_game_options_base base = game_options->base_options;
    susc_game_options_normal normal = game_options->mode.normal;
    uint8_t version = base.version;

    if (version >= 8)
    {
        SUSC_REQUIRE(hazel_message_writer_uint8(writer, base.special_mode));
        SUSC_REQUIRE(hazel_message_writer_uint8(writer, base.rules_preset));
    }

    SUSC_REQUIRE(hazel_message_writer_uint8(writer, base.max_players));
    SUSC_REQUIRE(hazel_message_writer_uint32(writer, base.game_keywords));
    SUSC_REQUIRE(hazel_message_writer_uint8(writer, base.map_type));
    SUSC_REQUIRE(hazel_message_writer_single(writer, normal.player_speed_mod));
    SUSC_REQUIRE(hazel_message_writer_single(writer, normal.crew_light_mod));
    SUSC_REQUIRE(hazel_message_writer_single(writer, normal.impostor_light_mod));
    SUSC_REQUIRE(hazel_message_writer_single(writer, normal.kill_cooldown));
    SUSC_REQUIRE(hazel_message_writer_uint8(writer, (uint8_t)normal.num_common_tasks));
    SUSC_REQUIRE(hazel_message_writer_uint8(writer, (uint8_t)normal.num_long_tasks));
    SUSC_REQUIRE(hazel_message_writer_uint8(writer, (uint8_t)normal.num_short_tasks));
    SUSC_REQUIRE(hazel_message_writer_int32(writer, normal.num_emergency_meetings));
    SUSC_REQUIRE(hazel_message_writer_uint8(writer, (uint8_t)base.num_impostors));
    SUSC_REQUIRE(hazel_message_writer_uint8(writer, normal.kill_distance));
    SUSC_REQUIRE(hazel_message_writer_int32(writer, normal.discussion_time));
    SUSC_REQUIRE(hazel_message_writer_int32(writer, normal.voting_time));
    SUSC_REQUIRE(hazel_message_writer_bool(writer, base.is_defaults));
    SUSC_REQUIRE(hazel_message_writer_bool(writer, normal.emergency_cooldown));
    SUSC_REQUIRE(hazel_message_writer_bool(writer, normal.confirm_impostor));
    SUSC_REQUIRE(hazel_message_writer_bool(writer, normal.visual_tasks));
    SUSC_REQUIRE(hazel_message_writer_bool(writer, normal.anonymous_votes));
    SUSC_REQUIRE(hazel_message_writer_bool(writer, normal.task_bar_update));

    SUSC_REQUIRE(susc_role_options_collection_serialize(
        &normal.role_options, writer));

    return 0;
}

int susc_game_options_serialize(susc_game_options *game_options, 
                                hazel_message_writer *writer)
{
    hazel_message_writer_packed_int32(writer, 0);

    hazel_message_writer_uint8(writer, game_options->base_options.version);
    hazel_message_writer_start_message(writer, 0);
    hazel_message_writer_uint8(writer, game_options->base_options.gamemode);

    switch(game_options->base_options.gamemode)
    {
        case SUSC_GAMEMODE_NORMAL:
        case SUSC_GAMEMODE_NORMALFOOLS:
            return susc_game_options_normal_serialize(game_options, writer);
            break;
        case SUSC_GAMEMODE_HIDENSEEK:
        case SUSC_GAMEMODE_SEEKFOOLS:
            break;
        default:
            break;
    }

    hazel_message_writer_end_message(writer);

    return 0;
}

int susc_game_options_deserialize(susc_game_options *game_options, 
                                  hazel_message_reader *reader)
{
    // TODO
    return 0;
}
