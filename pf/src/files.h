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

void get_files_in_confdir(char *directory);
/*
 filename structure
*/
typedef struct ConfigFilesStruct ConfigFilesStruct_t;
typedef struct Stack Stack_t;

/*
 Stack functions
*/
bool createFilestack(Stack_t **stack);
// stack is empty?
size_t isEmpty(Stack_t *stack);
bool pushFileStack(Stack_t **stack, ConfigFilesStruct_t *cf);
bool popFileStack(Stack_t **stack, ConfigFilesStruct_t *cf);
bool deleteFileStack(Stack_t **stack);
void displayFileStack(Stack_t *stack);

#endif
