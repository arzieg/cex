#include <stdio.h>
#include <string.h>
#include <glib.h>

int main()
{
    GHashTable *hash = g_hash_table_new(g_str_hash, g_str_equal);

    // some values
    gint *v_one = g_new(gint, 1), *v_two = g_new(gint, 1), *v_three = g_new(gint, 1);
    *v_one = 1, *v_two = 2, *v_three = 3;

    g_hash_table_insert(hash, "ONE", v_one);
    g_hash_table_insert(hash, "TWO", v_two);
    g_hash_table_insert(hash, "THREE", v_three);
    printf("\nThere are %d keys in the hash\n", g_hash_table_size(hash));
    printf("Key TWO %d\n", *(int *)g_hash_table_lookup(hash, "TWO"));
    g_hash_table_destroy(hash);
    return 0;
}
