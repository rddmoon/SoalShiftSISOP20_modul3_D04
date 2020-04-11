#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <ctype.h>

struct name{
  char path[500];
  char file[100];
};

char *getext(char *name) {
  char *ext = strrchr(name, '.');
  if(ext == NULL){
    return "";
  }
  return ext + 1;
}

char *getname(char *path) {
  char *name = strrchr(path, '/');
  if(name == NULL){
    return "";
  }
  return name++;
}

void lowercase(char *ext){
  int i;
  for (i = 0; i < strlen(ext); i++){
      ext[i] = tolower(ext[i]);
  }
}

void* moving(void* param){
  struct name *n = param;
  char file1[700];
  char file2[700];
  char *ext = getext(n->file);
  lowercase(ext);
  if(ext == ""){
    mkdir("Unknown", 0777);
    sprintf(file2, "./Unknown/%s", n->file);
  }
  else{
    mkdir(ext, 0777);
    sprintf(file2, "./%s/%s", ext, n->file);
  }
  //sprintf(file1, "%s/%s", n->path, n->file);
  strcpy(file1, n->path);
  rename(file1, file2);
  free(n);
}

int main(int argc, char* argv[]){
  DIR* dir;
  struct dirent *d;
  int i;
  char drct[700];

  if(argc == 2){
    if(strcmp(argv[1], "*") == 0){
      strcpy(drct, ".");
      if((dir = opendir(drct)) == NULL){
        printf("ERROR: gagal mengakses directory\n");
        exit(EXIT_FAILURE);
      }
    }
    else{
      printf("ERROR: argumen yang diberikan tidak sesuai\n");
      exit(EXIT_FAILURE);
    }
  }
  else if(argc > 2){
    if(argc == 3 && strcmp(argv[1], "-d") == 0){
      strcpy(drct, argv[2]);
      if((dir = opendir(drct)) == NULL){
        printf("ERROR: gagal mengakses directory\n");
        exit(EXIT_FAILURE);
      }
    }
    else if(strcmp(argv[1], "-f") == 0){
      pthread_t threads[argc];
      for(i = 2; i < argc; i++){
        char *n = getname(argv[i]);
        if(n == ""){
          printf("ERROR: argumen yang diberikan tidak sesuai\n");
          exit(EXIT_FAILURE);
        }
        struct name *f = (struct name *)malloc(sizeof(struct name));
        strcpy(f->file, n);
        strcpy(f->path, argv[i]);
        pthread_create(&threads[i], NULL, moving, (void *)f);
      }
      for(i = 2; i < argc; i++){
          pthread_join(threads[i], NULL);
      }
      exit(EXIT_SUCCESS);
    }
    else{
      printf("ERROR: argumen yang diberikan tidak sesuai\n");
      exit(EXIT_FAILURE);
    }
  }
  else{
    printf("ERROR: argumen yang diberikan tidak sesuai\n");
    exit(EXIT_FAILURE);
  }
  int count = 0;
  int cnt = 0;
  while((d = readdir(dir)) != NULL){
    if(d->d_type == DT_REG){
      count++;
    }
  }
  rewinddir(dir);
  pthread_t threads[count];
  while((d = readdir(dir)) != NULL){
    if(d->d_type != DT_REG){
      continue;
    }
    if(strcmp(argv[0]+2, d->d_name) == 0){
      threads[cnt] = 0;
      cnt++;
      continue;
    }
    struct name *f = (struct name *)malloc(sizeof(struct name));
    strcpy(f->file, d->d_name);
    sprintf(f->path, "%s/%s", drct, f->file);
    //strcpy(f->path, drct);
    pthread_create(&threads[cnt], NULL, moving, (void *)f);
    cnt++;
  }
  for(i = 0; i < count; i++){
    if(threads[i]){
      pthread_join(threads[i], NULL);
    }
  }
  exit(EXIT_SUCCESS);
}
