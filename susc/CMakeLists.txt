add_library(susc STATIC
    src/gamecode.c
    src/sus.c
    src/platform.c
    src/client/auth.c
    src/client/client.c
    src/client/game_listing.c
    src/client/version.c
    src/data/game_options/game_options.c
    src/data/game_options/role_options.c
    src/data/game_options/role_rate.c
    src/data/net_object.c
    src/data/net_object_types/lobby_behaviour.c
    src/data/net_object_types/network_transform.c
    src/data/net_object_types/player_control.c
    src/data/net_object_types/player_info.c
    src/data/net_object_types/player_physics.c
    src/game/event.c
    src/game/net_object_container.c
    src/game/state_internal.c
    src/game/state.c
    src/net/messages/c2s/hello.c
    src/net/messages/c2s/host_game.c
    src/net/messages/c2s/join_game.c
    src/utils/math.c
    src/utils/utils.c
    src/utils/vector.c
)
target_include_directories(susc
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        #${HAZEL_INCLUDE_DIRS}
)
if(MSVC)
  target_compile_options(susc PRIVATE /W4)
else()
  target_compile_options(susc PRIVATE -Wall -Wextra -Wpedantic -Wno-unused)
endif()

target_link_libraries(susc hazelnetworking::hazelnetworking)


target_compile_definitions(susc PUBLIC
  SUSC_LOGGING_ENABLE
  SUSC_LOGGING_DEBUG)