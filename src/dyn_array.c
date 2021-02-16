#include "dyn_array.h"
#include <stdlib.h>
#include <string.h>

void dyn_array_create(struct dyn_array* self, int element_size)
{
    self->size = 0;
    self->reserved = 10;
    self->element_size = element_size;
    self->data = calloc(self->reserved, element_size);
}

void dyn_array_push_back(struct dyn_array* self, void* element)
{
    if (self->size == self->reserved)
    {
        self->reserved *= 2;
        self->data = realloc(self->data, self->reserved);
        // Todo: check if realloc fails
    }
    const int offset = self->size * self->element_size;
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

bool dyn_array_set(struct dyn_array* self, int index, void* element)
{
    if (index >= self->size)
    {
        return false;
    }
    const int offset = self->size * self->element_size;
    char* data = self->data;
    memcpy(&data[offset], element, self->element_size);
    return true;
}

void* dyn_array_get(struct dyn_array* self, int index)
{
    if (index >= self->size)
    {
        return NULL;
    }
    const int offset = index * self->element_size;
    char* data = self->data;
    return &data[offset];
}

void dyn_array_destroy(struct dyn_array* self)
{
    free(self->data);
}