#pragma once

#include <stdint.h>

/**
 * \brief Game server authentication method
 */
enum susc_auth_method
{
    SUSC_AUTH_METHOD_NONCE,
    SUSC_AUTH_METHOD_DTLS
};

/**
 * \brief Game server authentication data
 * This is the data used to authenticate with the game server.
 * 
 * On private servers, you will want to use #SUSC_AUTH_METHOD_NONCE for #method.
 * 
 * #susc_auth_zero_nonce exists for private servers with no authentication 
 * (e.g. Impostor)
 */
typedef struct susc_auth_data
{
    enum susc_auth_method method;
    union
    {
        uint32_t nonce;
        char* matchmaker_token;
    } data;
} susc_auth_data;

/**
 * \brief Authentication data using a nonce set to zero.
 * This can be used to connect to private servers, such as Impostor, which do no
 * authentication.
 */
extern const susc_auth_data susc_auth_zero_nonce;
