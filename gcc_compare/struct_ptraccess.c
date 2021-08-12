#include <stdio.h>

struct type_t
{
    int a;
    int b;
    int c;
};

void print(struct type_t t)
{
    printf("%d, %d, %d\n", t.a, t.b, t.c);
}

int main()
{
    printf("--- pointer access --\n");
    struct type_t value;
    struct type_t *ptr = &value;

    value.a = 1;
    value.b = 2;
    value.c = 3;
    print(*ptr);
    print(value);

    printf("--- int array access --\n");
    ((int*)&value)[1]++;
    print(value);

    printf("--- char array access --\n");
    ((char*)&value)[1]++;
    print(value);

    printf("--- getting ridiculous ---\n");
    int diff = (int)&value.c - (int)&value;
    printf("diff: %d\n", diff);
    *(int*)((void*)ptr + diff) = 1337;
    print(value);

    return 0;
}
