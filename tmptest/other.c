
int vector_test()
{
    uint8_t buf[4];
    hazel_message_writer writer;
    hazel_message_writer_init(&writer, buf, sizeof(buf));

    susc_vector vector_test = {
        .x = 20.5f,
        .y = -15.9f
    };
    int ret;

    ret = susc_write_vector(vector_test, &writer);
    if (ret != 0)
    {
        return ret;
    }

    SUSC_LOG_DEBUG_PRINT_BYTES("vector_test", buf, sizeof(buf), 0);

    hazel_message_reader reader;
    hazel_message_reader_init(&reader, buf, sizeof(buf), 0);
    
    susc_vector vector_result;
    ret = susc_read_vector(&vector_result, &reader);
    if (ret != 0)
    {
        return ret;
    }

    printf("vector input : %f %f\n", vector_test.x, vector_test.y);
    printf("vector result: %f %f\n", vector_result.x, vector_result.y);
    return 0;
}

int no_test()
{
    susc_net_object child1_c3 = {
        .net_id = 13,
        ._child = NULL,
        ._next = NULL,
    };
    susc_net_object child1_c2 = {
        .net_id = 12,
        ._child = NULL,
        ._next = &child1_c3
    };
    susc_net_object child1_c1 = {
        .net_id = 11,
        ._child = NULL,
        ._next = &child1_c2
    };

    susc_net_object root = {
        .net_id = 1,
        ._child = &child1_c1,
        ._next = NULL
    };

    SUSC_NO_ITERATE_CHILDREN(&root, child)
    {
        printf("child %d\n", child->net_id);
    }

    return 0;
}