#pragma once

#include <stdint.h>

/** \defgroup client_version Client Version
 * @{
 */

typedef uint32_t susc_version;

/**
 * \brief Calculates the version number code from the year, month, day and
 *        revision the game
 */
#define SUSC_VERSION(year, month, day, revision) \
    (year * 25000) + (month * 1800) + (day * 50) + revision

/**
 * \brief Calculates the version number code from the year, month, day and
 *        revision of the game
 * This function simply returns the value of SUSC_VERSION with the same
 * arguments.
 */
susc_version susc_version_calc(int32_t year, int32_t month, 
                               int32_t day,  int32_t revision);

/**
 * \brief Extract the year component from the ::susc_version
 */
uint32_t susc_version_get_year    (susc_version version);
/**
 * \brief Extract the month component from the ::susc_version
 */
uint32_t susc_version_get_month   (susc_version version);
/**
 * \brief Extract the day component from the ::susc_version
 */
uint32_t susc_version_get_day     (susc_version version);
/**
 * \brief Extract the revision component from the ::susc_version
 */
uint32_t susc_version_get_revision(susc_version version);

/** @}*/
