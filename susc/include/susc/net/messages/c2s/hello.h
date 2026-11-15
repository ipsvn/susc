#pragma once
#include "../../../common.h"

#include "susc/chat_mode.h"
#include "susc/platform.h"
#include "susc/client/auth.h"
#include "susc/client/language.h"
#include "susc/client/version.h"

#include "hazel/reader.h"
#include "hazel/writer.h"

int susc_c2s_hello_serialize(hazel_message_writer *writer, 
                             susc_version version, 
                             const char *player_name,
                             susc_auth_data auth_data, 
                             enum susc_client_language language,
                             enum susc_chat_mode chat_mode,
                             susc_platform *platform);

// int susc_c2s_hello_deserialize(hazel_message_reader *reader,
//                                susc_version *version,
//                                char *player_name,
//                                susc_auth_data *auth_data,
//                                enum susc_client_language *language,
//                                enum susc_chat_mode *susc_chat_mode,
//                                susc_platform *platform);
