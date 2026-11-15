#pragma once

#include "susc/common.h"

#include "utils.h"

#include "hazel/reader.h"

#define SUSC_READER_TYPECONV(ret, reader, from, set) \
    do                                                \
    {                                                 \
        from##_t hazel_reader_typeconv_tmp;           \
        ret |= hazel_message_reader_##from(           \
            reader, &hazel_reader_typeconv_tmp);      \
        set = hazel_reader_typeconv_tmp;              \
    } while (0)

#define SUSC_READER_TYPECONV_REQUIRE_EXIT(reader, from, set, x)     \
    do                                                              \
    {                                                               \
        int susc_require_ret = 0;                                   \
        SUSC_READER_TYPECONV(susc_require_ret, reader, from, set);  \
        if (susc_require_ret != 0)                                  \
        {                                                           \
            x;                                                      \
        }                                                           \
    } while (0)

#define SUSC_READER_TYPECONV_REQUIRE(reader, from, set)         \
    SUSC_READER_TYPECONV_REQUIRE_EXIT(reader, from, set,        \
                                      return susc_require_ret)

#define SUSC_WRITER_STACK(writer_var, size) \
    hazel_message_writer writer_var;        \
    uint8_t buffer[size];                   \
    hazel_message_writer_init(&writer_var, buffer, sizeof(buffer));

static void reader_print_state(hazel_message_reader *reader)
{
    size_t starting_pos = reader->_position > 16 ? reader->_position - 16 : 0;
    size_t end_pos = reader->_position + 32;
    for (size_t i = starting_pos; i < end_pos; i++)
    {
        if (i >= reader->size) printf(" ??");
        else printf(" %02x", reader->data[i + reader->offset]);
    }
    printf("\n");
    for (size_t i = starting_pos; i < reader->read_head; i++)
    {
        printf("   ");
    }
    printf(" ^^ read head\n");
}
#define SUSC_READER_PRINT_STATE(reader) \
    reader_print_state(reader)
