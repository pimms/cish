#include <stdio.h>
#include <stdlib.h>


struct person_t
{
    const char *name;
    int age;
    int id;
    int height;
};



void print_person(const struct person_t *person)
{
    printf("{name: %s, age: %d, id: %d, height: %d}\n",
           person->name, person->age, person->id, person->height);
}

struct person_t set_name(struct person_t person, const char *name)
{
    printf("setting name to %s\n", name);
    person.name = name;
    return person;
}

struct person_t set_age(struct person_t person, int age)
{
    printf("setting age to %d\n", age);
    person.age = age;
    return person;
}

struct person_t set_id(struct person_t person, int id)
{
    printf("setting id to %d\n", id);
    person.id = id;
    return person;
}

struct person_t set_height(struct person_t person, int height)
{
    printf("setting height to %d\n", height);
    person.height = height;
    return person;
}

int main()
{
    struct person_t person;
    person = set_name(set_age(set_id(set_height(person, 186), 1337), 27), "Joakim");
    print_person(&person);
    return 0;
}

