#ifndef dyn_array_h
#define dyn_array_h

#include <stdbool.h>

struct dyn_array
{
    int size;
    int reserved;
    int element_size;
    void* data;
};

void dyn_array_create(struct dyn_array* self, int element_size);
void dyn_array_push_back(struct dyn_array* self, void* element);
void dyn_array_pop_back(struct dyn_array* self);
bool dyn_array_set(struct dyn_array* self, int index, void* element);
void* dyn_array_get(struct dyn_array* self, int index);
void dyn_array_destroy(struct dyn_array* self);

#endif