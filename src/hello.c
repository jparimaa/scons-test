#include <stdio.h>
#include <errno.h>
#include <string.h>

int main()
{
    FILE* fp = fopen("./input.txt", "r");
    if (fp == NULL)
    {
        printf("%s\n", strerror(errno));
        return 1;
    }
    int a, b, c;
    const int numbers_read = fscanf(fp, "%d %d %d", &a, &b, &c);
    fclose(fp);
    printf("Read %d numbers\n", numbers_read);
    printf("%d %d %d\n", a, b, c);
    return 0;
}