#include <stdio.h>
#include <stdlib.h>

typedef void (*fptr_animal_walk)(void*);

//

struct animal
{
    void* derived;
    fptr_animal_walk walk;
    int number;
};

void general_walk(int number)
{
    printf("Animal walking %d\n", number);
}

void animal_walk(void* a)
{
    struct animal* animal_ptr = (struct animal*)a;
    general_walk(animal_ptr->number);
}

struct animal* animal_create()
{
    struct animal* a = malloc(sizeof(struct animal));
    a->walk = animal_walk;
    a->number = 42;
    return a;
}

//

struct cat
{
    struct animal* base;
    float number;
};

void cat_walk(void* c)
{
    struct animal* animal_ptr = (struct animal*)c;
    struct cat* cat_ptr = (struct cat*)animal_ptr->derived;
    printf("Cat walking %f\n", cat_ptr->number);
}

struct cat* cat_create()
{
    struct cat* c = malloc(sizeof(struct cat));
    c->base = animal_create();
    c->base->derived = (void*)c;
    c->base->walk = cat_walk;
    c->number = 99.9f;
    return c;
}

//

struct dog
{
    struct animal* base;
    int number;
};

void dog_walk(void* d)
{
    struct animal* animal_ptr = (struct animal*)d;
    struct dog* dog_ptr = (struct dog*)animal_ptr->derived;
    general_walk(dog_ptr->number);
}

struct dog* dog_create()
{
    struct dog* d = malloc(sizeof(struct dog));
    d->base = animal_create();
    d->base->derived = (void*)d;
    d->base->walk = dog_walk;
    d->number = 306;
    return d;
}

//

struct general_animal
{
    struct animal* base;
};

struct general_animal* general_animal_create()
{
    struct general_animal* ga = malloc(sizeof(struct general_animal));
    ga->base = animal_create();
    ga->base->derived = (void*)ga;
    return ga;
}

//

int main()
{
    struct cat* c1 = cat_create();
    struct dog* d1 = dog_create();
    struct general_animal* ga1 = general_animal_create();

    struct animal* animals[3];
    animals[0] = c1->base;
    animals[1] = d1->base;
    animals[2] = ga1->base;

    for (int i = 0; i < 3; ++i)
    {
        animals[i]->walk(animals[i]);
    }

    // Todo: free allocated memory with virtual destroy-function

    return 0;
}