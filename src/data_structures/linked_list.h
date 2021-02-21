#ifndef linked_list_h
#define linked_list_h

#include <stddef.h>
#include <stdbool.h>

struct linked_list_node
{
    struct linked_list_node* previous;
    struct linked_list_node* next;
    void* data;
};

struct linked_list
{
    size_t size;
    size_t element_size;
    struct linked_list_node* begin;
    struct linked_list_node* end;
};

struct linked_list* linked_list_create(size_t element_size);
void linked_list_push_back(struct linked_list* self, const void* element);
bool linked_list_remove(struct linked_list* self, const struct linked_list_node* n);
void linked_list_destroy(struct linked_list* self);

#endif