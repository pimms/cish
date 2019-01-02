#include <stdio.h>
#include <stdlib.h>

struct person_t
{
    int age;
    const char *name;
};

int main()
{
    struct person_t me;
    me.name = "Joakim Stien";
    me.age = 45;

    struct person_t corrected;
    corrected = me;
    corrected.age = 27;

    struct person_t corrected2 = me;
    corrected2.age = 27;

    printf("me: %s, %d\n", me.name, me.age);
    printf("corrected: %s, %d\n", corrected.name, corrected.age);
    printf("corrected2: %s, %d\n", corrected2.name, corrected2.age);

    return 0;
}
