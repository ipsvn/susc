#pragma once

#include "susc/game/client_data.h"

struct susc_state_client_list
{
    susc_client_data client;
    struct susc_state_client_list *next;
};
