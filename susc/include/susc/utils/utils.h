#pragma once

#include "susc/common.h"

#ifndef SUSC_NO_PRIVATE
#define SUSC_PRIVATE(varname) hiplsdonttouchthis_##varname
#else
#define SUSC_PRIVATE(varname)
#endif

bool susc_cmpsid_uint16(uint16_t a, uint16_t b);
