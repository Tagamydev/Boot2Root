#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char buffer[] = "isrveawhobpnutfg";

void
validator(const char* str1, const char* str2)
{
    char arr[7] = {0};

    for (int i = 0; i < 6; i++)
        arr[i] = buffer[str1[i] & 0xf];

    if (strcmp(arr, str2) != 0)
        printf("[ KO ] output: %s\n", arr);
    else
        printf("[ OK ] output: %s\n", arr);
}

const int*
indexListGenerator(char const* str) {
    static int list[6] = {0};
    char* tmp;

    for (int i = 0; i < 6; i++) {
        if ((tmp = strchr(buffer, str[i])) == NULL)
            exit(1);
        list[i] = tmp - buffer;
    }

    printf("list of index for '%s': [%d, %d, %d, %d, %d, %d]\n"
        , str
        , list[0]
        , list[1]
        , list[2]
        , list[3]
        , list[4]
        , list[5]
    );
    return list;
}

const char*
arrayGenerator(const int* list)
{
    static char arr[7] = {0};

    for (int i = 0; i < 6; i++)
        arr[i] = list[i] | 0x30;

    printf("Generated array: '%s'\n", arr);
    return arr;
}

int
main(int argc, char** argv)
{
    if (argc != 2 || strlen(argv[1]) != 6)
        return 1;

    const int* list = indexListGenerator(argv[1]);
    const char* arr = arrayGenerator(list);
    validator(arr, argv[1]);
}