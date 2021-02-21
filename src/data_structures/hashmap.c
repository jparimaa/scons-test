#include "hashmap.h"
#include "linked_list.h"
#include <stdlib.h>
#include <string.h>

static const float c_fill_limit = 0.6f;

struct hash_info
{
    unsigned long id;
    struct linked_list* bucket;
};

static struct hash_info get_hash_info(const struct hashmap* h, const void* key)
{
    struct hash_info info;
    info.id = hash(key, h->key_size);
    const size_t bucket_index = info.id % h->reserved_buckets;
    const size_t offset = bucket_index * sizeof(struct linked_list);
    char* buckets_ptr = h->buckets;
    info.bucket = (struct linked_list*)(&buckets_ptr[offset]);
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
    self->buckets = calloc(self->reserved_buckets, sizeof(struct linked_list));
    return self;
}

void hashmap_set(struct hashmap* self, const void* key, const void* value)
{
    if ((float)self->used_buckets / (float)self->reserved_buckets > c_fill_limit)
    {
        self->reserved_buckets = (self->reserved_buckets * 4);
        self->buckets = realloc(self->buckets, self->reserved_buckets * sizeof(struct item));
    }

    struct hash_info info = get_hash_info(self, key);

    if (info.bucket != NULL)
    {
        struct linked_list_node* current = info.bucket->begin;
        while (current != NULL)
        {
            struct item* i = current->data;
            if (i->id == info.id)
            {
                memcpy(i->value, value, self->value_size);
                return;
            }
            current = current->next;
        }
    }
    else
    {
        info.bucket = linked_list_create(sizeof(struct item));
    }

    struct item new_item;
    new_item.id = info.id;
    new_item.value = malloc(self->value_size);
    memcpy(new_item.value, value, self->value_size);
    linked_list_push_back(info.bucket, &new_item);
    ++self->used_buckets;
}

bool hashmap_remove(struct hashmap* self, const void* key)
{
    struct hash_info info = get_hash_info(self, key);
    if (info.bucket == NULL)
    {
        return false;
    }

    struct linked_list_node* current = info.bucket->begin;
    while (current != NULL)
    {
        struct item* i = current->data;
        if (i->id == info.id)
        {
            free(i->value);
            linked_list_remove(info.bucket, current);
            if (info.bucket->size == 0)
            {
                --self->used_buckets;
            }
            return true;
        }
        current = current->next;
    }

    return false;
}

void* hashmap_get(struct hashmap* self, const void* key)
{
    struct hash_info info = get_hash_info(self, key);

    if (info.bucket == NULL)
    {
        return NULL;
    }

    struct linked_list_node* current = info.bucket->begin;
    const unsigned long id = hash(key, self->key_size);
    while (current != NULL)
    {
        struct item* i = current->data;
        if (i->id == id)
        {
            return i->value;
        }
        current = current->next;
    }
    return NULL;
}

void hashmap_destroy(struct hashmap* self)
{
    struct linked_list* list = self->buckets;
    for (size_t i = 0; i < self->reserved_buckets; ++i)
    {
        if (list == NULL)
        {
            continue;
        }

        struct linked_list_node* current = list->begin;
        while (current != NULL)
        {
            struct item* i = current->data;
            free(i->value);
            current = current->next;
        }
        linked_list_destroy(list);
        ++list;
    }
    free(self->buckets);
    free(self);
}