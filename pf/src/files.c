#include "files.h"

#include <dirent.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pf.h"

/*
typedef struct ConfigFilesStruct {
  char filename[256];
  InstallationType_t systemtype;
} ConfigFilesStruct_t;

typedef struct Stack {
  ConfigFilesStruct_t Configfile;
  struct Stack *next;
} Stack_t;
*/

bool createFilestack(Stack_t **stack) {
  if (*stack == NULL) return true;
  return false;
}

size_t isEmpty(Stack_t *stack) { return stack == NULL; }

// function to push an element onto the stack
bool pushFileStack(Stack_t **stack, ConfigFilesStruct_t *cf) {
  Stack_t *temp;
  temp = (Stack_t *)malloc(sizeof(Stack_t *));
  if (!temp) return false;
  strncpy(temp->Configfile.filename, cf->filename, strlen(cf->filename));
  temp->Configfile.systemtype = cf->systemtype;
  temp->next = *stack;
  *stack = temp;
  printf("%s is a %d pushed to the stack.\n", cf->filename, cf->systemtype);
  free(temp);
  return true;
}

// function to pop an element from the stack
bool popFileStack(Stack_t **stack, ConfigFilesStruct_t *cf) {
  if (isEmpty(*stack)) {
    printf("Stack underflow!\n");
    return false;
  }
  Stack_t *temp = *stack;
  if (isEmpty(temp)) return false;
  *stack = temp->next;
  strncpy(cf->filename, temp->Configfile.filename,
          strlen(temp->Configfile.filename));
  cf->systemtype = temp->Configfile.systemtype;
  free(temp);
  return true;
}

bool deleteFileStack(Stack_t **stack) {
  Stack_t *temp;
  while (*stack) {
    temp = (*stack)->next;
    free(*stack);
    *stack = temp;
  }
  return true;
}

// function to display the stack
void displayFileStack(Stack_t *stack) {
  if (isEmpty(stack)) {
    printf("Stack is empty!\n");
    return;
  }
  Stack_t *current = stack;
  printf("\nStack: ");
  while (current != NULL) {
    printf("\n%s is %d ", current->Configfile.filename,
           current->Configfile.systemtype);
    current = current->next;
  }
  printf("\n");
}

/*
// Test Main function

int testmain() {
  ConfigFilesStruct_t cf = {.filename = "ABC.conf", .systemtype = SCALEUP};
  Stack_t *filestack = NULL;

  strncpy(cf.filename, "C11.conf", 9);
  cf.systemtype = SCALEUP;
  pushFileStack(&filestack, &cf);
  strncpy(cf.filename, "B10.conf", 9);
  cf.systemtype = SCALEOUT;
  pushFileStack(&filestack, &cf);

  displayFileStack(filestack);

  popFileStack(&filestack, &cf);
  printf("popped from the stack, got %s and %d.\n\n", cf.filename,
         cf.systemtype);
  displayFileStack(filestack);
  popFileStack(&filestack, &cf);
  printf("popped from the stack, got %s and %d.\n\n", cf.filename,
         cf.systemtype);
  displayFileStack(filestack);
  deleteFileStack(&filestack);

  return 0;
}
*/

Stack_t *get_files_in_confdir(char *directory) {
  DIR *dir = opendir(directory);
  ConfigFilesStruct_t cf = {.filename = "ABC.conf", .systemtype = SCALEUP};
  Stack_t *ConfigFiles = NULL;

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
  strncpy(cf.filename, "/home/arne/dev/c/cex/pf/test/C11.conf", 38);
  cf.systemtype = SCALEUP;
  if (!pushFileStack(&ConfigFiles, &cf))
    fprintf(stderr, "Error pushing C11.conf to the stack");
  strncpy(cf.filename, "/home/arne/dev/c/cex/pf/test/B20.conf", 38);
  cf.systemtype = SCALEOUT;
  if (!pushFileStack(&ConfigFiles, &cf))
    fprintf(stderr, "Error pushing B20.conf to the stack");
  closedir(dir);
  printf("\nIn files.c ConfigFiles = %p", ConfigFiles);
  return ConfigFiles;
}