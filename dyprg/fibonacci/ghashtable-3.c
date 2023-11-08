// ex‑ghashtable‑3.c
#include <glib.h>
#include <stdio.h>

void iterator(gpointer key, gpointer value, gpointer user_data)
{
    printf(user_data, *(gint *)key, *(gint *)value);
}
int main(int argc, char **argv)
{
    GHashTable *hash = g_hash_table_new(g_int_hash, g_int_equal);
    gint *k_one = g_new(gint, 1), *k_two = g_new(gint, 1), *k_three = g_new(gint, 1);
    *k_one = 1, *k_two = 2, *k_three = 3;
    gint *v_one = g_new(gint, 1), *v_two = g_new(gint, 1), *v_three = g_new(gint, 1);
    *v_one = 1, *v_two = 4, *v_three = 9;
    g_hash_table_insert(hash, k_one, v_one);
    g_hash_table_insert(hash, k_two, v_two);
    g_hash_table_insert(hash, k_three, v_three);
    g_hash_table_foreach(hash, (GHFunc)iterator, "The square of %d is %d\n");
    g_hash_table_destroy(hash);
    return 0;
}