/* Einlesen von einem String */
#include <curses.h>
#define MAX 100

int main(void)
{
    char string1[MAX];
    char string2[MAX];

    initscr();
    printw("Bitte Eingabe machen :");
    getnstr(string1, MAX);
    mvprintw(5, 10, "Eingabe machen");
    mvgetnstr(5, 30, string2, MAX);
    endwin();
    printf("Die 1. Eingabe lautet :%s\n", string1);
    printf("Die 2. Eingabe lautet :%s\n", string2);
    endwin();
    return 0;
}