#include <stdlib.h>
#include <string.h>
#include "list.h"

// List init
void list_init(List *list, void (*destroy)(void *data))
{
    list->size = 0;
    list->destroy = destroy;
    list->head = NULL;
    list->tail = NULL;
    return;
}

// List destroy
void list_destroy(List *list)
{
    void *data;
    while (list_size(list) > 0)
    {
        if (list_rem_next(list, NULL, (void **)&data) == 0 && list->destroy != NULL)
        {
            list->destroy(data);
        }
    }
    memset(list, 0, sizeof(List));
    return;
}

// list_ins_next
int list_ins_next(List *list, ListElmt *element, const void *data)
{
    ListElmt *new_element;
    if ((new_element = (ListElmt *)malloc(sizeof(ListElmt))) == NULL)
        return -1;

    new_element->data = (void *)data;
    if (element == NULL)
    {
        if (list_size(list) == 0)
            list->tail = new_element;

        new_element->next = list->head;
        list->head = new_element;
    }
    else
    {
        // Handle insertation somewhere other than at the head
        if (element->next == NULL)
            list->tail = new_element;
        new_element->next = element->next;
        element->next = new_element;
    }
    list->size++;
    return 0;
}

// list_rem_next
int list_rem_next(List *list, ListElmt *element, void **data)
{
    ListElmt *old_element;

    // do not allow removel from empty list
    if (list_size(list) == 0)
        return -1;

    if (element == NULL)
    {
        // remove from head
        *data = list->head->data;
        old_element = list->head;
        list->head = list->head->next;

        if (list_size(list) == 1)
            list->tail = NULL;

        else
        {
            // remove from somewhere other than the head
            if (element->next == NULL)
                return -1;

            *data = element->next->data;
            old_element = element->next;
            element->next = element->next->next;

            if (element->next == NULL)
                list->tail = element;
        }
        free(old_element);
        list->size--;
        return 0;
    }
}