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
#include <string.h>

#define COUNT 100

struct s_matrix
{
    char *key[6];
    int value;
};

struct s_matrix matrix[COUNT];

long gT(int columns, int rows, int iter);

int main()
{
    int n, m, iter = 0;
    unsigned long paths;

    printf("\nTraveler Problem:\n");
    printf("\nAnzahl Spalten: ");
    scanf("%d", &n);
    printf("\nAnzahl Zeilen: ");
    scanf("%d", &m);
    paths = gT(n, m, iter);
    printf("\nAnzahl Pfade: %ld\n", paths);

    return 0;
}

long gT(int col, int row, int iter)
{
    char tmp[6]; /* key = "m,n" */
    sprintf(tmp, "%d,%d", row, col);
    printf("\n String = %s", tmp);

    /*if (key in memo)
        return memo(key);
*/
    if (col == 1 || row == 1)
        return 1;
    if (col == 0 || row == 0)
        return 0;

    *matrix[iter].key = tmp;
    matrix[iter].value = 5;

    // memo[key] = (gT(col - 1, row, hash) + gT(col, row - 1, hash));
    // return memo[key];

    return 0;
}
