#include "hashmap.h"
#include <stdlib.h>
#include <string.h>

static const float c_fill_limit = 0.6f;

static struct linked_list** get_bucket(const struct hashmap* h, unsigned long id)
{
    const size_t bucket_index = id % h->reserved_buckets;
    return &h->buckets[bucket_index];
}

static void reallocate(struct hashmap* h)
{
    struct hashmap new_hashmap = *h;
    new_hashmap.reserved_buckets = (h->reserved_buckets * 4);
    new_hashmap.used_buckets = 0;
    new_hashmap.buckets = calloc(new_hashmap.reserved_buckets, sizeof(struct linked_list*));

    for (size_t i = 0; i < h->reserved_buckets; ++i)
    {
        struct linked_list* list = h->buckets[i];
        if (list == NULL)
        {
            continue;
        }

        struct linked_list_node* current = list->begin;
        while (current != NULL)
        {
            struct item* current_item = current->data;
            struct linked_list** bucket = get_bucket(&new_hashmap, current_item->id);
            if (*bucket == NULL)
            {
                *bucket = linked_list_create(sizeof(struct item));
                ++new_hashmap.used_buckets;
            }
            linked_list_push_back(*bucket, current_item);
            current = current->next;
        }
        linked_list_destroy(list);
    }
    free(h->buckets);
    *h = new_hashmap;
}

// Public

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
    self->buckets = calloc(self->reserved_buckets, sizeof(struct linked_list*));
    return self;
}

void hashmap_set(struct hashmap* self, const void* key, const void* value)
{
    if ((float)self->used_buckets / (float)self->reserved_buckets > c_fill_limit)
    {
        reallocate(self);
    }

    const unsigned long id = hash(key, self->key_size);
    struct linked_list** bucket = get_bucket(self, id);

    if (*bucket != NULL)
    {
        struct linked_list_node* current = (*bucket)->begin;
        while (current != NULL)
        {
            struct item* i = current->data;
            if (i->id == id)
            {
                memcpy(i->value, value, self->value_size);
                return;
            }
            current = current->next;
        }
    }
    else
    {
        *bucket = linked_list_create(sizeof(struct item));
    }

    struct item new_item = {id, malloc(self->value_size)};
    memcpy(new_item.value, value, self->value_size);
    linked_list_push_back(*bucket, &new_item);
    ++self->used_buckets;
}

bool hashmap_remove(struct hashmap* self, const void* key)
{
    const unsigned long id = hash(key, self->key_size);
    struct linked_list** bucket = get_bucket(self, id);

    if (*bucket == NULL)
    {
        return false;
    }

    struct linked_list_node* current = (*bucket)->begin;
    while (current != NULL)
    {
        struct item* i = current->data;
        if (i->id == id)
        {
            free(i->value);
            linked_list_remove(*bucket, current);
            if ((*bucket)->size == 0)
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
    const unsigned long id = hash(key, self->key_size);
    struct linked_list** bucket = get_bucket(self, id);

    if (*bucket == NULL)
    {
        return NULL;
    }

    struct linked_list_node* current = (*bucket)->begin;
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
    for (size_t i = 0; i < self->reserved_buckets; ++i)
    {
        struct linked_list* list = self->buckets[i];
        if (list == NULL)
        {
            continue;
        }

        struct linked_list_node* current = list->begin;
        while (current != NULL)
        {
            struct item* current_item = current->data;
            free(current_item->value);
            current = current->next;
        }
        linked_list_destroy(list);
    }
    free(self->buckets);
    free(self);
}