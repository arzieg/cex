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

bool createFilestack(ConfigFilesStruct_t **stack) {
  *stack = NULL;
  return true;
}

size_t isEmpty(ConfigFilesStruct_t *stack) { return stack == NULL; }

// function to push an element onto the stack
bool pushFileStack(ConfigFilesStruct_t **stack, char *pfilename,
                   InstallationType_t psystemtype) {
  ConfigFilesStruct_t *newFile;
  newFile = (ConfigFilesStruct_t *)malloc(sizeof(ConfigFilesStruct_t *));
  if (!newFile) return false;
  newFile->filename = pfilename;
  newFile->systemtype = psystemtype;
  newFile->next = *stack;
  *stack = newFile;
  printf("%s is a %d pushed to the stack.\n", pfilename, psystemtype);
  return true;
}

// function to pop an element from the stack
bool popFileStack(ConfigFilesStruct_t **stack, char **pfilename,
                  InstallationType_t *psystemtype) {
  if (isEmpty(*stack)) {
    printf("Stack underflow!\n");
    return false;
  }
  ConfigFilesStruct_t *temp = *stack;
  if (!temp) return false;
  *stack = temp->next;
  *pfilename = temp->filename;
  *psystemtype = temp->systemtype;
  free(temp);
  return true;
}

bool deleteFileStack(ConfigFilesStruct_t **stack) {
  ConfigFilesStruct_t *temp;
  while (*stack) {
    temp = (*stack)->next;
    free(*stack);
    *stack = temp;
  }
  return true;
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
// Test Main function
*/
int testmain() {
  ConfigFilesStruct_t *filestack = NULL;
  char *pfilename;
  InstallationType_t psystemtype = SCALEUP;

  pushFileStack(&filestack, "C11.conf", SCALEUP);
  pushFileStack(&filestack, "B10.conf", SCALEOUT);
  displayFileStack(filestack);
  popFileStack(&filestack, &pfilename, &psystemtype);
  printf("popped from the stack, got %s and %d.\n\n", pfilename, psystemtype);
  displayFileStack(filestack);
  popFileStack(&filestack, &pfilename, &psystemtype);
  printf("popped from the stack, got %s and %d.\n\n", pfilename, psystemtype);
  displayFileStack(filestack);
  deleteFileStack(&filestack);

  return 0;
}

void get_files_in_confdir(char *directory) {
  DIR *dir = opendir(directory);
  ConfigFilesStruct_t *ConfigFiles = NULL;
  char *pfilename;
  InstallationType_t psystemtype = SCALEUP;

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
  printf("\n FAKE DATA \n");
  if (!createFilestack(&ConfigFiles)) fprintf(stderr, "Could not create Stack");
  if (!pushFileStack(&ConfigFiles, "C11.conf", SCALEUP))
    fprintf(stderr, "Error pushing C11.conf to the stack");
  if (!pushFileStack(&ConfigFiles, "B10.conf", SCALEOUT))
    fprintf(stderr, "Error pushing B10.conf to the stack");

  closedir(dir);
  return EXIT_SUCCESS;

  // zurückgegeben werden müsste nun der Zeiger auf ConfigFiles,
  // das geht an main zurück und von da muss dann die nächste Funktion wieder
  // übergeben werden.
}