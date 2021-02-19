#ifndef linked_list_h
#define linked_list_h

#include <stddef.h>

struct linked_list
{
    void* data;
};

struct linked_list* linked_list_create(size_t element_size);
void linked_list_add(struct linked_list* self, const void* element);
void linked_list_remove(struct linked_list* self, const void* element);
void linked_list_destroy(struct linked_list* self);

#endif