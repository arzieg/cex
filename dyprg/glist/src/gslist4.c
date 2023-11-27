// last, nth and nth data
#include <glib.h>
#include <stdio.h>

int main()
{
    GSList *list = NULL;
    list = g_slist_append(list, "first");
    list = g_slist_append(list, "second");
    list = g_slist_append(list, "third");
    printf("\nThe last item is %s \n", (char *)g_slist_last(list)->data);
    printf("Now the item at index 1 the easy way: %s\n", (char *)g_slist_nth_data(list, 1));
    printf("And the next item after first item is %s\n", (char *)g_slist_next(list)->data);
    g_slist_free(list);
    return 0;
}