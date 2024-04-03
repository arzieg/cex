#include "files.h"

#include <dirent.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pf.h"

/*
 Types of Installations
*/
typedef enum {
  SCALEUP,
  SCALEOUT,
  TOOLSERVER,
  MAJORITYMAKER,
  ISCSI
} InstallationType_t;

typedef struct ConfigFilesStruct {
  char *filename;
  InstallationType_t systemtype;
  struct ConfigFilesStruct *next;
} CONFIGFILESSTRUCT;

CONFIGFILESSTRUCT *create_fileentry(char *pfilename,
                                    InstallationType_t psystemtype) {
  CONFIGFILESSTRUCT *f = (CONFIGFILESSTRUCT *)malloc(sizeof(CONFIGFILESSTRUCT));
  f->filename = pfilename;
  f->next = NULL;
  f->systemtype = psystemtype;
  return f;
}

// check if stack is empty
size_t isEmpty(CONFIGFILESSTRUCT *top) { return top == NULL; }

// function to push an elment onto the stack
void push(CONFIGFILESSTRUCT **top, char *pfilename,
          InstallationType_t psystemtype) {
  CONFIGFILESSTRUCT *newFile = create_fileentry(pfilename, psystemtype);
  newFile->next = *top;
  *top = newFile;
  printf("%s pushed to the stack.\n", pfilename);
}

// functio to pop an element from the stack
char *pop(CONFIGFILESSTRUCT **top) {
  if (isEmpty(*top)) {
    printf("Stack underflow!\n");
    return ERROR;
  }
  CONFIGFILESSTRUCT *temp = *top;
  char *popped_filename = temp->filename;
  InstallationType_t popped_type = temp->systemtype;
  *top = (*top)->next;
  free(temp);
  return popped_filename;  // zweiter Wert muss auch noch zurückgegeben werden
}

// function to display the stack
void displayStack(CONFIGFILESSTRUCT *top) {
  if (isEmpty(top)) {
    printf("Stack is empty!\n");
    return;
  }
  CONFIGFILESSTRUCT *current = top;
  printf("\nStack: ");
  while (current != NULL) {
    printf("%s ", current->filename);
    current = current->next;
  }
  printf("\n");
}

/*
// Main function
int main() {
    struct Node* top = NULL;
    push(&top, 10);
    push(&top, 20);
    push(&top, 30);
    displayStack(top);
    printf("%d popped from the stack.\n", pop(&top));
    displayStack(top);
    return 0;
}

*/
void get_files_in_confdir(char *directory) {
  DIR *dir = opendir(directory);
  CONFIGFILESSTRUCT ConfigFiles;

  // ToDo
  // Anlegen einer Struktur
  // char *filename
  // char type
  // als dynamische Liste
  // Rückgabe des Zeigers zur dynamischen Liste

  // als erstes aber erst einmal faken und
  // C11.conf und B20.conf zurückgeben, um environmentindex zu testen.

  if (dir == NULL) {
    fprintf(stderr, "error: %s: %s (errno = %d)\n", directory, strerror(errno),
            errno);
    exit(EXIT_FAILURE);
  }

  printf("\n GET FILENAMES \n\n");

  /* get conf files */
  struct dirent *file;
  while ((file = readdir(dir)) != NULL) {
    // check for *.conf
    if (strstr(file->d_name, ".conf") != NULL) {
      // check that is after *.conf nothing
      size_t len = strlen(file->d_name);
      if (len > 5 && strcmp(file->d_name + len - 5, ".conf") == 0) {
        printf("%s\n", file->d_name);
      }
    }
  }

  /* fake*/

  closedir(dir);
  return 0;
}