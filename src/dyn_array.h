#ifndef dyn_array_h
#define dyn_array_h

struct dyn_array
{
    int size;
    int elementSize;
    void* data;
};

struct dyn_array* dyn_array_create();
void dyn_array_push_back(struct dyn_array* da, void* element);
void dyn_array_pop_back(struct dyn_array* da, void* element);
void dyn_array_destroy(struct dyn_array* da);

#endif