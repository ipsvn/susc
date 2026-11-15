#pragma once
#include "common.h"

#include <stdint.h>

#define SUSC_GAMECODE_INVALID_FORMAT -0xC001

/**
 * \brief Convert a string game 'name' to an integer gamecode
 * \param input input string game 'name'
 * \param output pointer to store output integer gamecode into
 * \return #SUSC_GAMECODE_INVALID_FORMAT if the input is invalid
 * \return otherwise, \c 0 on success.
 */
int susc_gamecode_name_to_int(const char* input, int32_t* output);

/**
 * \brief Convert an integer gamecode to a string game 'name'
 * \param input input integer gamecode
 * \param output pointer to store output string game 'name'.
 *               buffer should be at least \c 7 bytes (to store six character
 *               game name and null terminator)
 * \return #SUSC_GAMECODE_INVALID_FORMAT if the input is invalid
 * \return otherwise, \c 0 on success.
 */
int susc_gamecode_int_to_name(int32_t input, char* output);
