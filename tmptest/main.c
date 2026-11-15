#include "susc/platform.h"
#include "susc/data/net_object.h"
#include "susc/data/net_object_types/player_info.h"
#include "susc/data/net_object_types/player_control.h"
#include "susc/data/net_object_types/network_transform.h"
#include "susc/client/client.h"
#include "susc/gamecode.h"
#include "susc/utils/vector.h"
#include "susc/game/events.h"

#include <malloc.h>

#if defined(_WIN32)
#   include <winsock2.h>
#   include "mswsock.h"
#   include <ws2tcpip.h>
#   include <wspiapi.h>
#   pragma comment(lib, "Ws2_32.lib")

void initWSA()
{
    WSADATA wsaData;
    int ret = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (ret != 0) {
        printf("WSAStartup failed with error: %d\n", ret);
        exit(1);
    }
}
#endif

#define REQUIRE(m, f) \
    do                              \
    {                               \
        int susc_require_ret = (f); \
        if (susc_require_ret != 0)  \
        {                           \
            printf("%s failed: %d\n", m,\
                   susc_require_ret);   \
            return susc_require_ret;\
        }                           \
    } while (0)

int main(int argc, char **argv)
{
    int ret;

#if defined(_WIN32)
    initWSA();
#endif

    {
        susc_version version = 50607300;
        printf("Version: %d.%d.%d.%d\n",
            susc_version_get_year(version),
            susc_version_get_month(version),
            susc_version_get_day(version),
            susc_version_get_revision(version));
    }

    int32_t gc;
    REQUIRE("susc_gamecode_name_to_int",
            susc_gamecode_name_to_int("AAAAAA", &gc));
    printf("game code %d\n", gc);

    {
        char out[7];
        REQUIRE("susc_gamecode_int_to_name",
            susc_gamecode_int_to_name(gc, out));
        printf("game name %s\n", out);
    }

    susc_platform platform = {
        .name = "TESTNAME",
        .type = SUSC_PLATFORM_STEAM,
        .platform_id = 0
    };

    susc_client client;
    susc_client_init(
        &client,
        "susc",
        &platform,
        SUSC_VERSION(2024, 8, 10, 0),
        &susc_auth_zero_nonce
    );
    if ((ret = susc_client_connect(&client, "127.0.0.1", 22023,
                                   HAZEL_IP_MODE_IPV4)) != 0)
    {
        printf("susc_client_connect fail: %d\n", ret);
        return ret;
    }

    //int32_t game_code;
    //REQUIRE("susc_gamecode_name_to_int",
    //        susc_gamecode_name_to_int(argv[1], &game_code));

    REQUIRE("susc_client_join_game",
            susc_client_join_game(&client, gc));

    while(true)
    {
        susc_event_base* event;
        ret = susc_client_loop(&client, &event);
        if (ret != 0)
        {
            printf("loop failed: %d\n", ret);
            return ret;
        }

        if (event == NULL)
        {
            continue;
        }

        printf("Event type: %d\n", event->type);

        switch (event->type)
        {
        case SUSC_EVENT_JOINED_GAME:
        {
            printf("Joined game!\n");
            break;
        }
        case SUSC_EVENT_PLAYER_SPAWNED:
        {
            susc_event_player_spawned *spawned_event
                = (susc_event_player_spawned *)event;
            printf("Player spawned %d %d %s\n",
                   spawned_event->client->id,
                   client.state.client_id,
                   spawned_event->client->player_name);

            if (spawned_event->client->id == client.state.client_id)
            {
                printf("It's us!\n");

                susc_no_player_control_cmd_check_name(
                    spawned_event->client->character, &client.state,
                    "susc");
                susc_no_player_control_cmd_check_color(
                    spawned_event->client->character, &client.state,
                    SUSC_PLAYER_COLOR_PINK);

                susc_no_player_control_rpc_set_pet(
                    spawned_event->client->character, &client.state,
                    SUSC_PC_PET_EMPTY_ID);

                susc_no_player_control_rpc_set_hat(
                    spawned_event->client->character, &client.state,
                    SUSC_PC_HAT_NONE_ID);

                susc_no_player_control_rpc_set_skin(
                    spawned_event->client->character, &client.state,
                    "skin_screamghostface");

                susc_no_player_control_rpc_set_visor(
                    spawned_event->client->character, &client.state,
                    SUSC_PC_VISOR_EMPTY_ID);

                susc_no_player_control_rpc_set_nameplate(
                    spawned_event->client->character, &client.state,
                    SUSC_PC_NAMEPLATE_NONE_ID);

                susc_no_player_control_rpc_set_level(
                    spawned_event->client->character, &client.state,
                    69);
            }
        }
        case SUSC_EVENT_PLAYER_EVENT:
        {
            printf("player event\n");
            susc_event_player_event *player_event
                = (susc_event_player_event *)event;
            switch (player_event->player_event_type)
            {
                case SUSC_PLAYER_EVENT_RPC_SEND_CHAT:
                {
                    susc_event_player_rpc_send_chat *rpc_event
                        = (susc_event_player_rpc_send_chat *)event;
                    printf("Chat message: %s\n", rpc_event->message);

                    susc_no_player_control *my_pc
                        = susc_state_my_client(&client.state)->character;

                    float snap_x, snap_y;
                    if (sscanf(rpc_event->message, "snap %f %f", &snap_x, &snap_y) == 2)
                    {
                        printf("want to snap\n");
                        susc_no_network_transform *transform
                            = susc_no_player_control_get_transform(my_pc);
                        if (transform == NULL)
                        {
                            printf("Transform is null\n");
                            continue;
                        }

                        susc_vector position = {
                            .x = snap_x,
                            .y = snap_y
                        };
                        susc_no_network_transform_rpc_snap_to(transform, &client.state, position);
                    }

                    char buffer[128];
                    snprintf(buffer, 128, "you just said %s!", rpc_event->message);

                    susc_no_player_control_rpc_send_chat(
                        my_pc, &client.state,
                        buffer);
                }
            }
        }
        }

        printf("free %p\n", event);
        susc_event_free(event);
        event = NULL;

    }
}
