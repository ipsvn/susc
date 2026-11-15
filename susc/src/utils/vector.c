#include "susc/utils/vector.h"

#include "utils.h"
#include "hazel_utils.h"
#include "math.h"

#include "hazel/reader.h"
#include "hazel/writer.h"

const float LERP_MIN = -50.f;
const float LERP_MAX =  50.f;

int susc_read_vector(susc_vector *vec, hazel_message_reader *reader)
{
    uint16_t a, b;
    SUSC_REQUIRE(hazel_message_reader_uint16(reader, &a));
    SUSC_REQUIRE(hazel_message_reader_uint16(reader, &b));

    susc_vector tmp = {
        .x = susc_lerp(LERP_MIN, LERP_MAX, a / 65535.f),
        .y = susc_lerp(LERP_MIN, LERP_MAX, b / 65535.f)
    };

    *vec = tmp;
    return 0;
}

int susc_write_vector(susc_vector vec, hazel_message_writer *writer)
{
    uint16_t a = (uint16_t)(susc_reverse_lerp(LERP_MIN, LERP_MAX, vec.x) * 65535.f);
    uint16_t b = (uint16_t)(susc_reverse_lerp(LERP_MIN, LERP_MAX, vec.y) * 65535.f);
    SUSC_REQUIRE(hazel_message_writer_uint16(writer, a));
    SUSC_REQUIRE(hazel_message_writer_uint16(writer, b));
    return 0;
}
