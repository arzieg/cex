#include <dirent.h>
#include <stdio.h>
#include <string.h>

#define NORMAL "\xB[0m"
#define GREEN "\x1B[32m"
#define BLUE "\x1B[34m"

void get_dir(char *path) {
  DIR *d = opendir(path);
  if (d == NULL) return;
  struct dirent *dir;
  while ((dir = readdir(d)) != NULL) {
    if (dir->d_type != DT_DIR)
      printf("%s%s\n", BLUE, dir->d_name);
    else if (dir->d_type == DT_DIR && strcmp(dir->d_name, ".") != 0 &&
             strcmp(dir->d_name, "..") != 0) {
      printf("%s%s\n", GREEN, dir->d_name);
      char d_path[255];
      sprintf(d_path, "%s/%s", path, dir->d_name);
      get_dir(d_path);
    }
  }
  closedir(d);
}

int main(int argc, char **argv) {
  printf("%s", NORMAL);
  get_dir(argv[1]);
  printf("%s\n", NORMAL);
  return 0;
}