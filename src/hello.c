#include "dyn_array.h"
#include <stdio.h>

struct my_struct
{
    int a;
    int b;
    int c;
};

int main()
{
    struct dyn_array my_array;
    dyn_array_create(&my_array, sizeof(struct my_struct));

    {
        struct my_struct data;
        data.a = 10;
        data.b = 20;
        data.c = 30;
        dyn_array_push_back(&my_array, &data);
        data.a = 42;
        dyn_array_push_back(&my_array, &data);
    }

    for (int i = 0; i < my_array.size; ++i)
    {
        struct my_struct* data = dyn_array_get(&my_array, i);
        printf("%d %d %d\n", data->a, data->b, data->c);
    }

    dyn_array_destroy(&my_array);

    return 0;
}