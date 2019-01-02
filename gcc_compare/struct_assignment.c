#include <stdio.h>

struct person_t
{
    const char *name;
    int age;
    int ssn;
};


void print_person(const struct person_t *person)
{
    printf("{name: %s, age: %d, ssn: %d}\n", person->name, person->age, person->ssn);
}

int main()
{
    struct person_t me;
    me.name = "Joakim Stien";
    me.age = 27;
    me.ssn = 42060;

    struct person_t copy = me;
    copy.age = 30;

    struct person_t copy2;
    copy2 = me;
    copy2.age = 25;
    copy2.name = "Almost Joakim Stien";

    print_person(&me);
    print_person(&copy);
    print_person(&copy2);

    struct person_t *ptr = &me;
    struct person_t copyFromPtr = *ptr;

    ptr->name = "Hi Mom";

    print_person(&me);
    print_person(ptr);
    print_person(&copyFromPtr);

    return 0;
}
