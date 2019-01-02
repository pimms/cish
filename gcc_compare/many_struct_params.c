#include <stdio.h>

struct item_t
{
    int value;
    const char *name;
};


struct item_t get_apple()
{
    struct item_t item;
    item.value = 19;
    item.name = "apple";
    return item;
}

struct item_t get_car()
{
    struct item_t item;
    item.value = 299000;
    item.name = "very nice car";
    return item;
}

struct item_t get_house()
{
    struct item_t item;
    item.value = 3000000;
    item.name = "very nice house";
    return item;
}

void describe_items(struct item_t i0, struct item_t i1, struct item_t i2)
{
    printf("[item0: %s, %d]\n", i0.name, i0.value);
    printf("[item1: %s, %d]\n", i1.name, i1.value);
    printf("[item2: %s, %d]\n", i2.name, i2.value);
}

int main()
{
    describe_items(get_apple(), get_car(), get_house());
    return 0;
}
