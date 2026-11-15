#include "susc/client/client.h"

#include "susc/errors.h"
#include "susc/net/messages/tags.h"
#include "susc/net/messages/c2s/hello.h"
#include "susc/net/messages/c2s/host_game.h"
#include "susc/net/messages/c2s/join_game.h"

#include "../utils/hazel_utils.h"

void _susc_client_state_handle_send(
    susc_game_state *state, hazel_message_writer *writer);

int susc_client_init(susc_client *client,
                     const char *player_name,
                     susc_platform *platform,
                     susc_version version,
                     const susc_auth_data *auth_data)
{
    int ret;
    if ((ret = susc_state_init(&client->state, player_name, platform)) != 0)
    {
        return ret;
    }

    client->state.handle_send = _susc_client_state_handle_send;

    client->version = version;
    client->auth_data = *auth_data;

    if ((ret = susc_event_queue_init(&client->state.event_queue)) != 0)
    {
        return ret;
    }

    return 0;
}

int susc_client_connect(susc_client *client, 
                        const char *hostname, uint16_t port,
                        enum hazel_ip_mode ip_mode)
{
    int ret = 0;
    if ((ret = hazel_udp_client_init(&client->udp_client, 
                                     hostname, port, ip_mode)) != 0)
    {
        return ret;
    }

    uint8_t handshake_buffer[128];
    hazel_message_writer handshake_writer;
    if ((ret = hazel_message_writer_init(&handshake_writer, handshake_buffer, 
                                         sizeof(handshake_buffer))) != 0)
    {
        return ret;
    }
    if ((ret = susc_client_write_hello_data(client, &handshake_writer)) != 0)
    {
        return ret;
    }

    SUSC_LOG_DEBUG_PRINT_BYTES("handshake_writer", handshake_writer.data, 
                                handshake_writer.size, 0);

    if ((ret = hazel_udp_client_handshake(
        &client->udp_client, handshake_writer.data, handshake_writer.size)) 
        != HAZEL_UDP_CLIENT_HANDSHAKE_SUCCESS)
    {
        printf("handshake fail %d\n", ret);
        return ret;
    }

    return ret;
}

int susc_client_host_game(susc_client *client, susc_game_options *options)
{
    uint8_t buffer[128];
    hazel_message_writer writer;
    SUSC_REQUIRE(hazel_message_writer_init(
        &writer, buffer, sizeof(buffer)));
    SUSC_REQUIRE(hazel_message_writer_clear(
        &writer, HAZEL_SEND_OPTION_RELIABLE));

    SUSC_REQUIRE(susc_c2s_host_game_serialize(&writer, options, true));

    susc_client_send_message(client, &writer);
    
    return 0;
}

int susc_client_join_game(susc_client *client, int32_t game_code)
{
    client->state.game_id = game_code;
    
    uint8_t buffer[128];
    hazel_message_writer writer;
    SUSC_REQUIRE(hazel_message_writer_init(
        &writer, buffer, sizeof(buffer)));
    SUSC_REQUIRE(hazel_message_writer_clear(
        &writer, HAZEL_SEND_OPTION_RELIABLE));

    SUSC_REQUIRE(susc_c2s_join_game_serialize(&writer, game_code, true));

    susc_client_send_message(client, &writer);

    return 0;
}

int susc_client_handle_message(
    susc_client *client, hazel_message_reader *reader)
{
    SUSC_LOG_DEBUG("handle message %d", reader->tag);

    switch (reader->tag)
    {
        default:
            return susc_state_handle_message(&client->state, reader);
    }
}

int susc_client_loop(susc_client *client, susc_event_base **event)
{
    hazel_message_reader reader;
    enum hazel_send_option send_option;

    int ret;

    ret = susc_event_queue_pop(&client->state.event_queue, event);
    if (ret == 0)
    {
        // passthrough event
        return 0;
    }

    ret = hazel_udp_client_recv(&client->udp_client, &send_option, &reader);
    if (ret == HAZEL_UDP_CLIENT_RECV_NO_MESSAGE)
    {
        return 0;
    }

    if (ret == HAZEL_UDP_CLIENT_RECV_HAS_DISCONNECTED)
    {
        printf("we got disconnected!\n");
        return 1;
    }

    if(ret == HAZEL_UDP_CLIENT_RECV_HAS_MESSAGE)
    {
        while (hazel_message_reader_get_position(&reader) < reader.size)
        {
            hazel_message_reader sub_reader;
            SUSC_REQUIRE(hazel_message_reader_read_message(
                &reader, &sub_reader));
            // SUSC_LOG_DEBUG_PRINT_BYTES("handling message", sub_reader.data, sub_reader.size, sub_reader.offset);
            int tr = susc_client_handle_message(client, &sub_reader);
            if (tr != 0)
            {
                printf("susc_client_handle_message returned %d\n", tr);
            }
        }

    }
    return 0;
}

int susc_client_write_hello_data(susc_client *client,
                                 hazel_message_writer *writer)
{
    return susc_c2s_hello_serialize(
        writer,
        client->version,
        client->state.player_name,
        client->auth_data,
        client->language,
        client->chat_mode,
        &client->state.platform
    );
}

void _susc_client_state_handle_send(
    susc_game_state *state, hazel_message_writer *writer)
{
    susc_client_send_message((susc_client *)state, writer);
}
int susc_client_send_message(susc_client *client, hazel_message_writer *writer)
{
    return hazel_udp_connection_send(
        &client->udp_client.udp_connection, writer);
}
