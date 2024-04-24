#ifndef FILESLIST
#define FILESLIST

#include <stdbool.h>
#include <stdlib.h>

typedef enum {
  SCALEUP,
  SCALEOUT,
  TOOLSERVER,
  MAJORITYMAKER,
  ISCSI
} InstallationType_t;

typedef struct ConfigFilesStruct {
  char filename[256];
  InstallationType_t systemtype;
} ConfigFilesStruct_t;

typedef struct Stack {
  ConfigFilesStruct_t Configfile;
  struct Stack *next;
} Stack_t;

/*
 filename structure

typedef struct ConfigFilesStruct ConfigFilesStruct_t;
typedef struct Stack Stack_t;
*/
/*
 Stack functions
*/
Stack_t *get_files_in_confdir(char *directory);
bool createFilestack(Stack_t **stack);
// stack is empty?
size_t isEmpty(Stack_t *stack);
bool pushFileStack(Stack_t **stack, ConfigFilesStruct_t *cf);
bool popFileStack(Stack_t **stack, ConfigFilesStruct_t *cf);
bool deleteFileStack(Stack_t **stack);
void displayFileStack(Stack_t *stack);

#endif
