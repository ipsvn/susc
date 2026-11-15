#pragma once

#include "susc/data/net_object.h"

#include "susc/data/net_object_types/lobby_behaviour.h"
#include "susc/data/net_object_types/player_control.h"
#include "susc/data/net_object_types/player_info.h"

const struct susc_net_object_prefab *susc_default_spawnable_no[] = {
    NULL, // 0  SkeldShipStatus
    NULL, // 1  MeetingHud
    &susc_no_lobby_behaviour_type, // 2  LobbyBehaviour
    &susc_no_lobby_behaviour_type,  // 3  LobbyBehaviour
    &susc_no_player_control_type,   // 4  PlayerControl
    NULL, // 5  MiraShipStatus
    NULL, // 6  PolusShipStatus
    NULL, // 7  DleksShipStatus
    NULL, // 8  AirshipStatus
    NULL, // 9  HideAndSeekManager
    NULL, // 10 NormalGameManager
    &susc_no_player_info_type,      // 11 NetworkedPlayerInfo
    NULL, // 12 VoteBanSystem
    NULL, // 13 FungleShipStatus
};
