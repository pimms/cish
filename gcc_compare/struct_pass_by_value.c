#include <stdio.h>
#include <stdlib.h>

struct item_t
{
    int a;
    int b;
};

int sum(struct item_t item)
{
    return item.a + item.b;
}

int double_sum(struct item_t item)
{
    item.a *= 2;
    item.b *= 2;
    return sum(item);
}

int main()
{
    struct item_t item1;
    item1.a = 100;
    item1.b = 100;

    printf("item1 2xsum: %d\n", double_sum(item1));
    printf("item1 sum: %d\n", sum(item1));

    return 0;
}

