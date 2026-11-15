#include "susc/client/auth.h"

const susc_auth_data susc_auth_zero_nonce =
{
    .method = SUSC_AUTH_METHOD_NONCE,
    .data.nonce = 0
};
