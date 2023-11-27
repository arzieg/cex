#include <stdio.h>

char *titles[] = {
    "A Tale of Two Cities",
    "Wuthering Heights",
    "Don Quixote",
    "Odyssey",
    "Mobby-Dick",
    "Hamlet",
    "Gulliver's Travel"};

int main()
{
    char **bestBooks[3];
    char **englishBooks[4];
    bestBooks[0] = &titles[0];
    bestBooks[1] = &titles[3];
    bestBooks[2] = &titles[5];

    englishBooks[0] = &titles[0];
    englishBooks[1] = &titles[1];
    englishBooks[2] = &titles[5];
    englishBooks[3] = &titles[6];

    printf("%s\n", *englishBooks[1]);
    printf("%p\n", &englishBooks[1]);
    printf("%s\n", titles[1]);
    printf("%p\n", &titles[1]);
}