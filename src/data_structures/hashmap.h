#ifndef hashmap_h
#define hashmap_h

#include <stddef.h>
#include <stdbool.h>

struct hashmap
{
    size_t reserved_buckets;
    size_t used_buckets;
    size_t key_size;
    size_t value_size;
    void* buckets;
};

struct item
{
    unsigned long id;
    void* value;
};

unsigned long hash(const char* data, size_t length);

struct hashmap* hashmap_create(size_t key_size, size_t value_size);
void hashmap_set(struct hashmap* self, const void* key, const void* value);
bool hashmap_remove(struct hashmap* self, const void* key);
void* hashmap_get(struct hashmap* self, const void* key);
void hashmap_destroy(struct hashmap* self);

#endif