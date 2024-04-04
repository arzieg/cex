#ifndef FILESLIST
#define FILESLIST

#include <stdlib.h>

typedef enum {
  SCALEUP,
  SCALEOUT,
  TOOLSERVER,
  MAJORITYMAKER,
  ISCSI
} InstallationType_t;

/*
 filename structure
*/
typedef struct ConfigFilesStruct ConfigFilesStruct_t;

ConfigFilesStruct_t *create_fileentry(char *pfilename, InstallationType_t);

// check if stack is empty
size_t isEmpty(ConfigFilesStruct_t *stack);

// function to push an elment onto the stack
void push(ConfigFilesStruct_t **stack, char *pfilename, InstallationType_t);
// functio to pop an element from the stack
char *pop(ConfigFilesStruct_t **stack);
// function to display the stack
void displayStack(ConfigFilesStruct_t *stack);

#endif