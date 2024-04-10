#include "files.h"

#include <dirent.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pf.h"

typedef struct ConfigFilesStruct {
  char *filename;
  InstallationType_t systemtype;
  struct ConfigFilesStruct *next;
} ConfigFilesStruct_t;

size_t createFilestack(ConfigFilesStruct_t **stack) {
  *stack = NULL;
  return EXIT_SUCCESS;
}

size_t isEmpty(ConfigFilesStruct_t *stack) { return stack == NULL; }

// function to push an elment onto the stack
size_t pushFileStack(ConfigFilesStruct_t **stack, char *pfilename,
                     InstallationType_t psystemtype) {
  ConfigFilesStruct_t *newFile;
  newFile = (ConfigFilesStruct_t *)malloc(sizeof(ConfigFilesStruct_t *));
  if (!newFile) return EXIT_FAILURE;
  newFile->filename = pfilename;
  newFile->systemtype = psystemtype;
  newFile->next = *stack;
  *stack = newFile;
  printf("%s is a %d pushed to the stack.\n", pfilename, psystemtype);
  return EXIT_SUCCESS;
}

// function to pop an element from the stack
ConfigFilesStruct_t *popFileStack(ConfigFilesStruct_t **stack) {
  if (isEmpty(*stack)) {
    printf("Stack underflow!\n");
    return EXIT_FAILURE;
  }
  ConfigFilesStruct_t *temp = *stack;

  *stack = temp->next;
  char *filename = temp->filename;
  InstallationType_t systemtype = temp->systemtype;

  free(temp);
  return EXIT_SUCCESS;
}

size_t deleteFileStack(ConfigFilesStruct_t **stack) {
  ConfigFilesStruct_t *temp;
  while (*stack) {
    temp = (*stack)->next;
    free(*stack);
    *stack = temp;
  }
  return EXIT_SUCCESS;
}

// function to display the stack
void displayFileStack(ConfigFilesStruct_t *stack) {
  if (isEmpty(stack)) {
    printf("Stack is empty!\n");
    return;
  }
  ConfigFilesStruct_t *current = stack;
  printf("\nStack: ");
  while (current != NULL) {
    printf("\n%s is %d ", current->filename, current->systemtype);
    current = current->next;
  }
  printf("\n");
}

/*
// Main function
*/
int testmain() {
  ConfigFilesStruct_t *filestack = NULL;
  char *pfilename;
  InstallationType_t *psystemtype;

  pushFileStack(&filestack, "C11.conf", SCALEUP);
  pushFileStack(&filestack, "B10.conf", SCALEOUT);
  displayFileStack(filestack);
  popFileStack(&filestack);
  printf("popped from the stack, got %s and %d.\n\n", pfilename, *psystemtype);
  displayFileStack(filestack);
  return 0;
}

void get_files_in_confdir(char *directory) {
  DIR *dir = opendir(directory);
  ConfigFilesStruct_t ConfigFiles;

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
  return EXIT_SUCCESS;
}