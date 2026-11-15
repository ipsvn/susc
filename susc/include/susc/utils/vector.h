#pragma once

typedef struct hazel_message_reader hazel_message_reader;
typedef struct hazel_message_writer hazel_message_writer;

typedef struct susc_vector
{
    float x, y;
} susc_vector;

int susc_read_vector(susc_vector *vec, hazel_message_reader *reader);
int susc_write_vector(susc_vector vec, hazel_message_writer *writer);
