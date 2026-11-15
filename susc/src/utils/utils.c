#include "susc/utils/utils.h"

bool susc_cmpsid_uint16(uint16_t a, uint16_t b)
{
    uint16_t num = b + 32767;
    if (b < num)
    {
        if (a > b) return a <= num;
        else return false;
    }
    if (a <= b)
    {
        return a <= num;
    }
    return true;
}
