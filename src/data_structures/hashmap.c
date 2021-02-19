#include "hashmap.h"
#include <stdlib.h>
#include <string.h>

static const float c_fill_limit = 0.6f;

struct hash_info
{
    unsigned long id;
    size_t bucket;
    struct item* item_ptr;
};

static struct hash_info get_hash_info(const struct hashmap* h, const void* key)
{
    struct hash_info info;
    info.id = hash(key, h->key_size);
    info.bucket = info.id % h->reserved_buckets;
    const size_t offset = info.bucket * h->value_size;
    char* data_ptr = h->data;
    info.item_ptr = (struct item*)(&data_ptr[offset]);
    return info;
}

unsigned long hash(const char* data, size_t length)
{
    unsigned long hash = 5381;
    for (size_t i = 0; i < length; ++i)
    {
        hash = ((hash << 5) + hash) + data[i]; /* hash * 33 + c */
    }

    return hash;
}

struct hashmap* hashmap_create(size_t key_size, size_t value_size)
{
    struct hashmap* self = calloc(1, sizeof(struct hashmap));
    self->key_size = key_size;
    self->value_size = value_size;
    self->reserved_buckets = 32;
    self->data = calloc(self->reserved_buckets, sizeof(struct item));
    return self;
}

bool hashmap_add(struct hashmap* self, const void* key, const void* value)
{
    if ((float)self->used_buckets / (float)self->reserved_buckets > c_fill_limit)
    {
        self->reserved_buckets = (self->reserved_buckets * 4);
        self->data = realloc(self->data, self->reserved_buckets * sizeof(struct item));
    }

    struct hash_info info = get_hash_info(self, key);

    if (info.item_ptr != NULL)
    {
        // Todo: handle collision
    }
    else
    {
        info.item_ptr->id = info.id;
        info.item_ptr->value = malloc(self->value_size);
        memcpy(info.item_ptr->value, value, self->value_size);
        ++self->used_buckets;
    }
    return true;
}

bool hashmap_remove(struct hashmap* self, const void* key)
{
    struct hash_info info = get_hash_info(self, key);

    if (info.item_ptr != NULL)
    {
        // Todo: handle multiple values
        free(info.item_ptr->value);
        memset(info.item_ptr, 0, sizeof(struct item));
        --self->used_buckets;
        return true;
    }
    return false;
}

void* hashmap_get(struct hashmap* self, const void* key)
{
    struct hash_info info = get_hash_info(self, key);
    return info.item_ptr->value;
}

void hashmap_destroy(struct hashmap* self)
{
    struct item* item_ptr = self->data;
    for (size_t i = 0; i < self->reserved_buckets; ++i)
    {
        free(item_ptr->value);
        ++item_ptr;
    }
    free(self->data);
    free(self);
}