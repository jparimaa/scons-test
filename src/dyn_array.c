#include "dyn_array.h"
#include <stdlib.h>
#include <string.h>

struct dyn_array* dyn_array_create(size_t element_size)
{
    struct dyn_array* self = calloc(1, sizeof(struct dyn_array));
    self->element_size = element_size;
    return self;
}

void dyn_array_push_back(struct dyn_array* self, void* element)
{
    if (self->size == self->reserved)
    {
        self->reserved = (self->reserved * 2) + 1;
        self->data = realloc(self->data, self->reserved * self->element_size);
    }
    const size_t offset = self->size * self->element_size;
    char* data = self->data;
    memcpy(&data[offset], element, self->element_size);
    ++self->size;
}

void dyn_array_pop_back(struct dyn_array* self)
{
    if (self->size == 0)
    {
        return;
    }
    --self->size;
}

void dyn_array_set(struct dyn_array* self, size_t index, void* element)
{
    const size_t offset = index * self->element_size;
    char* data = self->data;
    memcpy(&data[offset], element, self->element_size);
}

void* dyn_array_get(struct dyn_array* self, size_t index)
{
    const size_t offset = index * self->element_size;
    char* data = self->data;
    return &data[offset];
}

void dyn_array_destroy(struct dyn_array* self)
{
    free(self->data);
    free(self);
}