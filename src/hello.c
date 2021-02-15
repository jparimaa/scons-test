#include "dyn_array.h"
#include <stdio.h>

void func()
{
    printf("func called\n");
}

int main()
{
    struct dyn_array* my_da;
    dyn_array_create(my_da);

    int arr[] = {1, 2, 3, 4};
    printf("Hello\n");
    func();
    return 0;
}