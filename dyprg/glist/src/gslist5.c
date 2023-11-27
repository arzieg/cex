#include <glib.h>
#include <stdio.h>

typedef struct
{
    char *name;
    int shoe_size;
} Person;

int main()
{
    GSList *list = NULL;
    Person *tom = (Person *)malloc(sizeof(Person));
    tom->name = "Tom";
    tom->shoe_size = 12;
    list = g_slist_append(list, tom);

    Person *fred = g_new(Person, 1); // allocate memory for on Person
    fred->name = "Fred";
    fred->shoe_size = 11;
    list = g_slist_append(list, fred);

    printf("\nTom's shoe size is %d\n", ((Person *)list->data)->shoe_size);
    printf("The last Person's name is %s\n", ((Person *)g_slist_last(list)->data)->name);
    g_slist_free(list);
    free(tom);
    g_free(fred);
    return 0;
}