#include "data_structures/dyn_array.h"
#include <stdio.h>

struct my_struct
{
    int a;
    int b;
    int c;
};

int main()
{
    struct dyn_array* my_array = dyn_array_create(sizeof(struct my_struct));

    {
        struct my_struct tmp1;
        tmp1.a = 10;
        tmp1.b = 20;
        tmp1.c = 30;
        dyn_array_push_back(my_array, &tmp1);
        tmp1.a = 42;
        dyn_array_push_back(my_array, &tmp1);
        struct my_struct tmp2 = {1, 2, 3};
        for (int i = 0; i < 3; ++i)
        {
            dyn_array_push_back(my_array, &tmp2);
        }
        tmp1.a = 99;
        dyn_array_set(my_array, 3, &tmp1);
    }

    dyn_array_pop_back(my_array);

    for (size_t i = 0; i < my_array->size; ++i)
    {
        struct my_struct* data = dyn_array_get(my_array, i);
        printf("%d %d %d\n", data->a, data->b, data->c);
    }

    dyn_array_destroy(my_array);

    return 0;
}