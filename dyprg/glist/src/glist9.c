#include <stdio.h>
#include <glib.h>

gint my_comparator(gconstpointer item1, gconstpointer item2)
{
    return g_ascii_strcasecmp(item1, item2);
}

int main()
{
    GSList *list, *iterator;
    list = g_slist_append(NULL, "Chicago");
    list = g_slist_append(list, "Boston");
    list = g_slist_append(list, "Albany");
    list = g_slist_sort(list, (GCompareFunc)my_comparator);
    for (iterator = list; iterator; iterator = iterator->next)
    {
        printf("\nCurrent item is %s", (char *)iterator->data);
    }
    return 0;
}