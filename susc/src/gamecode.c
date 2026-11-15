#include "susc/gamecode.h"

#include "utils/utils.h"

#include <string.h>

static const char _v2[26] = "QWXRTYLPESDFGHUJKZOCVBINMA";
static int32_t _v2map[26] = {25,21,19,10,8,11,12,13,22,15,16,6,24,23,18,7,0,3,9,4,14,20,1,2,5,17};

int _int_to_name_v2(int32_t input, char* output)
{
    int32_t a = input & 0x3FF;
    if ((size_t)a / 26 >= ARRAY_LENGTH(_v2))
    {
        return SUSC_GAMECODE_INVALID_FORMAT;
    }
    int b = (input >> 10) & 0xFFFFF;
    output[0] = _v2[a % 26];
    output[1] = _v2[a / 26];
    output[2] = _v2[b % 26];
    output[3] = _v2[b / 26 % 26];
    output[4] = _v2[b / (26 * 26) % 26];
    output[5] = _v2[b / (26 * 26 * 26) % 26];
    output[6] = 0;
    return 0;
}

int _name_to_int_v2(const char* input, int32_t* output)
{
    for (size_t i = 0; i < 6; i++)
    {
        if (input[i] < 'A' || input[i] > 'Z')
        {
            return SUSC_GAMECODE_INVALID_FORMAT;
        }
    }
    int32_t a = _v2map[input[0] - 65];
    int32_t b = _v2map[input[1] - 65];
    int32_t c = _v2map[input[2] - 65];
    int32_t d = _v2map[input[3] - 65];
    int32_t e = _v2map[input[4] - 65];
    int32_t f = _v2map[input[5] - 65];

    int one = (a + 26 * b) & 0x3FF;
    int two = (c + 26 * (d + 26 * (e + 26 * f)));

    *output = (one | ((two << 10) & 0x3FFFFC00) | 0x80000000);
    return 0;
}

int susc_gamecode_name_to_int(const char* input, int32_t* output)
{
    size_t len = strlen(input);
    if (len == 6)
    {
        return _name_to_int_v2(input, output);
    }
    if (len != 4)
    {
        return SUSC_GAMECODE_INVALID_FORMAT;
    }
    *output = input[0] | ((input[1] | ((input[2] | (input[3] << 8)) << 8)) << 8);
    return 0;
}

int susc_gamecode_int_to_name(int32_t input, char* output)
{
    if (input < -1)
    {
        return _int_to_name_v2(input, output);
    }
    output[0] = (char)input;
    output[1] = (char)(input >> 8);
    output[2] = (char)(input >> 16);
    output[3] = (char)(input >> 24);
    output[4] = 0;
    return 0;
}
