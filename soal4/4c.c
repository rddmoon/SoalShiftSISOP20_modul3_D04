#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(){
  int fd[2];
  pipe(fd);

  pid_t child_id, child_id2;

  child_id = fork();
  if (child_id < 0) {
    exit(EXIT_FAILURE);
  }
  if (child_id == 0){
    //this is child
    close(fd[0]);
    dup2(fd[1], 1); //stdout write pipe
    close(fd[1]);
    execlp("ls","ls", NULL);
  }
  child_id2 = fork();
  if (child_id2 < 0) {
    exit(EXIT_FAILURE);
  }
  if (child_id2 == 0){
    //this is child
    close(fd[1]);
    dup2(fd[0], 0); //stdin read pipe
    close(fd[0]);
    execlp("wc","wc","-l", NULL);
  }
  //this is parent
  close(fd[0]);
  close(fd[1]);
  int i;
  for(i = 0; i < 2; i++){
    wait(NULL);
  }
}
