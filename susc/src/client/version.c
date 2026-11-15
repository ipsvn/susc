#include "susc/client/version.h"

susc_version susc_version_calc(int32_t year, int32_t month, 
                           int32_t day,  int32_t revision)
{
    return SUSC_VERSION(year, month, day, revision);
}

uint32_t susc_version_get_year(susc_version version)
{
    return version / 25000;
}
uint32_t susc_version_get_month(susc_version version)
{
    return (version % 25000) / 1800;
}
uint32_t susc_version_get_day(susc_version version)
{
    return ((version % 25000) % 1800) / 50;
}
uint32_t susc_version_get_revision(susc_version version)
{
    return ((version % 25000) % 1800) % 50;
}
