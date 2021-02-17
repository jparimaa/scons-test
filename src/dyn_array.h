#ifndef dyn_array_h
#define dyn_array_h

#include <stddef.h>

struct dyn_array
{
    size_t size;
    size_t reserved;
    size_t element_size;
    void* data;
};

struct dyn_array* dyn_array_create(size_t element_size);
void dyn_array_push_back(struct dyn_array* self, void* element);
void dyn_array_pop_back(struct dyn_array* self);
void dyn_array_set(struct dyn_array* self, size_t index, void* element);
void* dyn_array_get(struct dyn_array* self, size_t index);
void dyn_array_destroy(struct dyn_array* self);

#endif