#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef SUSC_LOGGING_ENABLE
#   define SUSC_LOG_ERROR(...) printf("\x1B[31m[%s:%d] ", __FILE__, __LINE__); printf(__VA_ARGS__); printf("\x1B[0m\n");
#   ifdef SUSC_LOGGING_DEBUG
#       define SUSC_LOG_DEBUG(...) printf("\x1B[37m[%s:%d] ", __FILE__, __LINE__); printf(__VA_ARGS__); printf("\x1B[0m\n");
#       define SUSC_LOG_DEBUG_PRINT_BYTES(tag, input, size, offset) printf ("\x1B[37m[%s:%d] %s (size:%lld) [", __FILE__, __LINE__, tag, size); for (size_t i = 0; i < size; i++) { printf ("%02x, ", input[i + offset]); } printf("]\x1B[0m\n");
#   endif
#endif

#ifndef SUSC_LOG_ERROR
#   define SUSC_LO``G_ERROR(...) ;
#endif

#ifndef SUSC_LOG_DEBUG
#   define SUSC_LOG_DEBUG(...) ;
#endif
#ifndef SUSC_LOG_DEBUG_PRINT_BYTES
#   define SUSC_LOG_DEBUG_PRINT_BYTES(tag, input, size, offset) ;
#endif

#define SUSC_MAX_PLAYER_NAME_LENGTH 10
#define SUSC_EOS_PRODUCTUSERID_MAX_LENGTH 32