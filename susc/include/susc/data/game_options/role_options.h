#pragma once

#include "susc/data/enums/role_types.h"

#include "role_rate.h"

#include "roles/engineer.h"
#include "roles/guardian_angel.h"
#include "roles/noisemaker.h"
#include "roles/phantom.h"
#include "roles/scientist.h"
#include "roles/shapeshifter.h"
#include "roles/tracker.h"

#include "hazel/reader.h"
#include "hazel/writer.h"

typedef struct susc_role_options
{
    enum susc_role_types role_type;
    union
    {
        susc_role_options_engineer engineer;
        susc_role_options_guardian_angel guardian_angel;
        susc_role_options_noisemaker noisemaker;
        susc_role_options_phantom phantom;
        susc_role_options_scientist scientist;
        susc_role_options_shapeshifter shapeshifter;
        susc_role_options_tracker tracker;
    } roles;
} susc_role_options;

typedef struct susc_role_data
{
    enum susc_role_types role_type;
    susc_role_options role_options;
    susc_role_rate role_rate;
    bool has_data;
} susc_role_data;

typedef struct susc_role_options_collection
{
    susc_role_data role_data[SUSC_ROLE_TYPES_COUNT];
} susc_role_options_collection;

int susc_role_options_collection_serialize(
    susc_role_options_collection *roles,
    hazel_message_writer *writer);
int susc_role_options_collection_deserialize(
    susc_role_options_collection *roles,
    hazel_message_reader *reader);
