// Test fuer key-value pair
#include <glib.h>
#include <stdio.h>

int main()
{
    gint *lookup = g_new(int, 1);

    GHashTable *hash = g_hash_table_new(g_int_hash, g_int_equal);
    gint *k_one = g_new(gint, 1), *k_two = g_new(gint, 1);
    gint *v_one = g_new(gint, 1), *v_two = g_new(gint, 1);
    *k_one = 1, *k_two = 2;
    *v_one = 11, *v_two = 4;
    g_hash_table_insert(hash, k_one, v_one);
    g_hash_table_insert(hash, k_two, v_two);

    printf("\nThere are %d keys in the hash\n", g_hash_table_size(hash));
    *lookup = 2;
    printf("Lookup for k_two = 2 is value %d\n", *(int *)g_hash_table_lookup(hash, lookup));
    g_hash_table_destroy(hash);
    return 0;
}
