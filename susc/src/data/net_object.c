#include "susc/data/net_object.h"

#include "../utils/utils.h"
#include <stdlib.h>

int susc_net_object_children_count(susc_net_object *net_object)
{
    int i = 0;
    SUSC_NO_ITERATE_CHILDREN(net_object, child)
    {
        i++;
    }
    return i;
}

susc_net_object *susc_net_object_find_child_of_type(
    susc_net_object *net_object, 
    enum susc_net_object_type type)
{
    SUSC_NO_ITERATE_CHILDREN(net_object, it)
    {
        if (it->_type == type)
        {
            return it;
        }
    }
    return NULL;
}

int susc_net_object_default_destroy(susc_net_object *net_object)
{
    // TODO
    // return 0;
    if (net_object == NULL) return 0;
    
    if (net_object->_child != NULL)
    {
        SUSC_REQUIRE(susc_net_object_default_destroy(net_object->_child));
    }

    if (net_object->_next != NULL)
    {
        SUSC_REQUIRE(susc_net_object_default_destroy(net_object->_next));
    }

    printf("free net_object %p\n", (void*)net_object);
    free(net_object);
    return 0;
}
