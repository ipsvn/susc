#pragma once
#include "../../../common.h"

#include "../../../data/game_options/game_options.h"

#include "hazel/reader.h"
#include "hazel/writer.h"

int susc_c2s_host_game_serialize(
    hazel_message_writer *writer,
    susc_game_options *game_options, bool enable_crossplay);
