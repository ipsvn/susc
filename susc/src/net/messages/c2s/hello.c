#include "susc/net/messages/c2s/hello.h"

#include "susc/client/version.h"

#include "../../../utils/utils.h"

static const susc_version _v1 = SUSC_VERSION(2021, 4, 25, 0);
static const susc_version _v2 = SUSC_VERSION(2021, 6, 30, 0);
static const susc_version _v3 = SUSC_VERSION(2021, 11, 9, 0);
static const susc_version _v4 = SUSC_VERSION(2021, 12, 14, 0);

int susc_c2s_hello_serialize(hazel_message_writer *writer, 
                             susc_version version, 
                             const char *player_name,
                             susc_auth_data auth_data, 
                             enum susc_client_language language,
                             enum susc_chat_mode chat_mode,
                             susc_platform *platform)
{
    int ret = 0;
    SUSC_REQUIRE(hazel_message_writer_int32(writer, version));
    SUSC_REQUIRE(hazel_message_writer_string(writer, player_name));
    if (auth_data.method == SUSC_AUTH_METHOD_DTLS)
    {
        SUSC_REQUIRE(hazel_message_writer_string(writer, auth_data.data.matchmaker_token));
    }
    else
    {
        SUSC_REQUIRE(hazel_message_writer_uint32(writer, 0));
    }
    SUSC_REQUIRE(hazel_message_writer_uint32(writer, language));
    SUSC_REQUIRE(hazel_message_writer_uint8(writer, chat_mode));
    SUSC_REQUIRE(susc_platform_serialize(platform, writer));
    if (auth_data.method == SUSC_AUTH_METHOD_DTLS)
    {
        SUSC_REQUIRE(hazel_message_writer_string(writer, ""));
    }
    else
    {
        SUSC_REQUIRE(hazel_message_writer_uint8(writer, 0));
        SUSC_REQUIRE(hazel_message_writer_uint32(writer, 0));
    }
    return ret;
}
