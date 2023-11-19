/* GridTravel in einer 2D Umgebung.
   Wieviele mögliche Wege gibt es in einer Matrix m * n, um von oben link nach unten rechts zu wandern?

   Beispiel:
     gridTraveler (2,3) -> 3 mögliche Wege. RRD, RDR, DRR (R=right, D=down)
     gridTraveler (1,1) -> 1 mögliche Wege.
     gridTraveler (0,1) -> 0 wenn eine Dimension 0 ist, dann ist die Lösung 0
     gridTraveler (1,0) -> 0 wenn eine Dimension 0 ist, dann ist die Lösung 0
     gridTraveler (3,3) -> Wenn man nun z.B. DRDR geht, dann ist jeweils eine Teillösung in
                           gridTraveler(3,3) -> gridTraveler(2,3) -> gridTraveler(2,2) -> gridTraveler(2,1) -> gridTraveler(1,1)
     gridTraveler (18,18)->2333606220

    Komplexität
      O(time)      2^(n+m)
      O(space)
   */
#include <stdio.h>
#include <string.h>
#include <glib.h>

GHashTable *hash;

long gT(int columns, int rows);

int main()
{
    int n, m;
    glong paths;
    // initialize hash table
    hash = g_hash_table_new(g_str_hash, g_int_equal);

    printf("\nTraveler Problem:\n");
    printf("\nAnzahl Spalten: ");
    scanf("%d", &n);
    printf("\nAnzahl Zeilen: ");
    scanf("%d", &m);
    paths = gT(n, m);
    printf("\nAnzahl Pfade: %ld\n", paths);

    return 0;
}

glong gT(int col, int row)
{
    glong *value = g_new(glong, 1);
    char key[6]; /* key = "m,n" */
    sprintf(key, "%d,%d", row, col);
    printf("\n Key = %s", key);

    // check if key in hash table
    if (g_hash_table_lookup(hash, key) != NULL)
        return *(glong *)g_hash_table_lookup(hash, key);
    if (col == 1 || row == 1)
        return 1;
    if (col == 0 || row == 0)
        return 0;

    *value = gT(col - 1, row) + gT(col, row - 1);
    g_hash_table_insert(hash, key, value);
    // memo[key] = (gT(col - 1, row, hash) + gT(col, row - 1, hash));
    return *value;

    return 0;
}
