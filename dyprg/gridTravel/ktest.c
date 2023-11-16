#include <stdio.h>
#include <string.h>
#include <glib.h>

void printKeyValue(gpointer key, gpointer value, gpointer userData)
{
    char *realKey = key;
    int realValue = *(int *)value;

    printf("%s => %d\n", realKey, realValue);

    return;
}

int main()
{
    GHashTable *hash = g_hash_table_new(g_str_hash, g_str_equal);

    // some values
    gint *v_one = g_new(gint, 1), *v_two = g_new(gint, 1), *v_three = g_new(gint, 1);
    *v_one = 1, *v_two = 2, *v_three = 3;

    gchar *k_one = g_new(gchar, 6), *k_two = g_new(gchar, 6), *k_three = g_new(gchar, 6);
    k_one = "ONE";
    k_two = "TWO";
    k_three = "THREE";

    g_hash_table_insert(hash, k_one, v_one);
    g_hash_table_insert(hash, k_two, v_two);
    g_hash_table_insert(hash, k_three, v_three);
    printf("\nThere are %d keys in the hash\n", g_hash_table_size(hash));
    printf("Key TWO = %d\n", *(int *)g_hash_table_lookup(hash, "TWO"));
    printf("\nIterate over Keys:\n");
    g_hash_table_foreach(hash, printKeyValue, NULL);
    g_hash_table_destroy(hash);
    return 0;
}
