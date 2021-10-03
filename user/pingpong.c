#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  
  int p[2];
  pipe(p);
  int pid = fork();
  if (pid == 0) {
    
  } else {

  }
  exit(0);
}