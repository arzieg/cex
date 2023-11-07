// gcc - I /usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -Wall -Wextra -g glibtest.c -lglib-2.0 -o glibtes

#include <glib.h>
#include <stdio.h>

int main()
{

    GHashTable *hash = g_hash_table_new(g_int_hash, g_int_equal);
    g_hash_table_insert(hash, "Virginia", "Richmond");
    printf(" %s ", g_hash_table_lookup(hash, "Virginia"));
    gboolean found = g_hash_table_remove(hash, "Virginia");
    printf("The Value 'virginia' was %s found and removed \n", found ? "" : "not");
    g_hash_table_destroy(hash);

    return 0;
}