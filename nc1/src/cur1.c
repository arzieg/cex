#include <curses.h>

int cbreak(void);  // Zeichenpuffer aus
int nobreak(void); // Zeichenpuffer an

int main(void)
{
    /* curses initialisieren*/
    initscr();

    /* curses beenden*/
    endwin();
    return 0;
}