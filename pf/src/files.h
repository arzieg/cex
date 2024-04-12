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

/*
 Stack functions
*/
bool createFilestack(ConfigFilesStruct_t **stack);
// stack is empty?
size_t isEmpty(ConfigFilesStruct_t *stack);
bool pushFileStack(ConfigFilesStruct_t **stack, char *filename,
                   InstallationType_t);
bool popFileStack(ConfigFilesStruct_t **stack, char **filename,
                  InstallationType_t *);
bool deleteFileStack(ConfigFilesStruct_t **stack);
void displayFileStack(ConfigFilesStruct_t *stack);

#endif
