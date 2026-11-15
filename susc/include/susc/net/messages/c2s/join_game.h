#pragma once
#include "../../../common.h"

#include "hazel/reader.h"
#include "hazel/writer.h"

int susc_c2s_join_game_serialize(hazel_message_writer *writer,
                                 int32_t game_code, bool enable_crossplay);

// int susc_c2s_hello_deserialize(hazel_message_reader *reader,
//                                susc_version *version,
//                                char *player_name,
//                                susc_auth_data *auth_data,
//                                enum susc_client_language *language,
//                                enum susc_chat_mode *susc_chat_mode,
//                                susc_platform *platform);
