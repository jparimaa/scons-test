#include "linked_list.h"
#include <stdlib.h>
#include <string.h>

struct linked_list* linked_list_create(size_t element_size)
{
    struct linked_list* self = calloc(1, sizeof(struct linked_list));
    self->element_size = element_size;
    return self;
}

void linked_list_push_back(struct linked_list* self, const void* element)
{
    struct linked_list_node* prev_end = self->end;
    self->end = calloc(1, sizeof(struct linked_list_node));
    self->end->data = calloc(1, self->element_size);
    memcpy(self->end->data, element, self->element_size);

    if (prev_end == NULL)
    {
        self->begin = self->end;
    }
    else
    {
        self->end->previous = prev_end;
        prev_end->next = self->end;
    }

    ++self->size;
}

bool linked_list_remove(struct linked_list* self, const struct linked_list_node* n)
{
    struct linked_list_node* current = self->begin;
    while (current != NULL)
    {
        if (current == n)
        {
            if (current->previous != NULL)
            {
                current->previous->next = current->next;
            }
            if (current->next != NULL)
            {
                current->next->previous = current->previous;
            }
            free(current->data);
            free(current);
            --self->size;
            return true;
        }
        current = current->next;
    }
    return false;
}

void linked_list_destroy(struct linked_list* self)
{
    struct linked_list_node* current = self->begin;
    while (current != NULL)
    {
        free(current->data);
        struct linked_list_node* tmp = current;
        current = current->next;
        free(tmp);
    }
    free(self);
}
