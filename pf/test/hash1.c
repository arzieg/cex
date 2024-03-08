#include <stdio.h>
#include <string.h>

// Größe der Hashtabelle
#define TABLE_SIZE 10

// Definition der Hashtabelle
struct HashTable {
    char key[20];
    int value;
};

// Hashfunktion: Berechnet den Index für einen Schlüssel
int hash_function(char* key) {
    int sum = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        sum += key[i];
    }
    return sum % TABLE_SIZE;
}

// Einfügen eines Schlüssels-Wert-Paares in die Hashtabelle
void insert(struct HashTable table[], char* key, int value) {
    int index = hash_function(key);
    strcpy(table[index].key, key);
    table[index].value = value;
    printf("\n key = %s, index=%d", key, index);
}

// Suchen eines Werts anhand des Schlüssels
int search(struct HashTable table[], char* key) {
    int index = hash_function(key);
    switch (index){
       case  0 : printf("\nein apfel"); break;
       case 3 : printf("\neine Kirsche"); break;
       case 9 : printf("\neine Banane"); break;

    }
    if (strcmp(table[index].key, key) == 0) {
        return table[index].value;
    }
    return -1; // Schlüssel nicht gefunden
}

int main() {
    // Initialisierung der Hashtabelle
    struct HashTable table[TABLE_SIZE];

    // Beispiel: Einfügen von Schlüssel-Wert-Paaren
    insert(table, "apple", 42);
    insert(table, "banana", 123);
    insert(table, "cherry", 99);

    // Beispiel: Suche nach einem Wert
    char searchKey[] = "banana";
    int result = search(table, searchKey);
    if (result != -1) {
        printf("Wert für Schlüssel '%s': %d\n", searchKey, result);
    } else {
        printf("Schlüssel '%s' nicht gefunden.\n", searchKey);
    }

    return 0;
}

