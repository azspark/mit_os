#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define READ 0
#define WRITE 1
#define stdin 0
#define stdout 1
#define stderr 2

void primeProc(int leftPipe[2]) {
    int first;
    close(leftPipe[WRITE]);
    if (read(leftPipe[READ], &first, sizeof(first)) < sizeof(first)) {
      exit(0);
    }
    fprintf(1, "prime %d\n", first);
    int rightPipe[2];
    pipe(rightPipe);
    int res = fork();
    if (res < 0) {
      exit(1);
    }
    if (res > 0) {
      close(rightPipe[READ]);
      int cur;
      while (read(leftPipe[READ], &cur, sizeof(cur)) == sizeof(cur)) {
        if (cur % first != 0) {
          write(rightPipe[WRITE], &cur, sizeof(cur));
        }
      }
      close(rightPipe[WRITE]);
      exit(0);
    } else {
      primeProc(rightPipe);
    }
}

int main(int argc, char *argv[]) {
  int parent_to_child[2];
  pipe(parent_to_child);
  int res = fork();
  if (res < 0) {
    fprintf(stderr, "Fork failed!");
    exit(1);
  }
  if (res > 0) {
    close(parent_to_child[READ]);
    int i;
    fprintf(1, "prime %d\n", 2);
    for (i = 2; i <= 35; i++) {
        if (i % 2 != 0) {
          write(parent_to_child[WRITE], &i, 4);
        }
    }
    close(parent_to_child[WRITE]);
    wait(0);
  } else {
    primeProc(parent_to_child);
  }
  exit(0);
}