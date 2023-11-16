/* GridTravel in einer 2D Umgebung.
   Wieviele mögliche Wege gibt es in einer Matrix m * n, um von oben link nach unten rechts zu wandern?

   Beispiel:
     gridTraveler (2,3) -> 3 mögliche Wege. RRD, RDR, DRR (R=right, D=down)
     gridTraveler (1,1) -> 1 mögliche Wege.
     gridTraveler (0,1) -> 0 wenn eine Dimension 0 ist, dann ist die Lösung 0
     gridTraveler (1,0) -> 0 wenn eine Dimension 0 ist, dann ist die Lösung 0
     gridTraveler (3,3) -> Wenn man nun z.B. DRDR geht, dann ist jeweils eine Teillösung in
                           gridTraveler(3,3) -> gridTraveler(2,3) -> gridTraveler(2,2) -> gridTraveler(2,1) -> gridTraveler(1,1)

    Komplexität
      O(time)      2^(n+m)
      O(space)
   */
#include <stdio.h>
#include <glib.h> // hash tables

long gT(int columns, int rows, GHashTable *hash);

int main()
{
    int n, m;
    unsigned long paths;

    GHashTable *hash = g_hash_table_new(g_str_hash, g_str_equal);

    printf("\nTraveler Problem:\n");
    printf("\nAnzahl Spalten: ");
    scanf("%d", &n);
    printf("\nAnzahl Zeilen: ");
    scanf("%d", &m);
    paths = gT(n, m, hash);
    printf("\nAnzahl Pfade: %ld\n", paths);

    return 0;
}

long gT(int col, int row, GHashTable *hash)
{
    // printf("\ncol=%d   row=%d ", col, row);
    // https: // www.delftstack.com/howto/c/concatenate-string-and-int-in-c/
    char *key[6] = m + "," + n;
    if (key in memo)
        return memo(key);

    if (col == 1 || row == 1)
        return 1;
    if (col == 0 || row == 0)
        return 0;

    memo[key] = (gT(col - 1, row, hash) + gT(col, row - 1, hash));
    return memo[key];
}