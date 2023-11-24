/* glist example */
#include <stdio.h>
#include <glib.h>

int main()
{
    GSList *value = NULL;
    GSList *iterator = NULL;
    gint *val1 = g_new(gint, 1);
    gint *val2 = g_new(gint, 1);
    *val1 = 1;
    *val2 = 2;
    printf("\nThe list is now %d items long", g_slist_length(value));
    value = g_slist_append(value, val1);
    value = g_slist_append(value, val2);
    printf("\nThe list is now %d items long\n", g_slist_length(value));
    for (iterator = value; iterator; iterator = iterator->next)
    {
        printf("Current item is '%d'\n", *(int *)iterator->data);
    }
    g_slist_free(value);

    return 0;
}