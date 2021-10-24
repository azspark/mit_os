#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define stdin 0
#define stderr 2
#define MAX_ARG_CHAR 512

char* argvBuf[MAXARG];
char buf[MAX_ARG_CHAR];

int main(int argc, char* argv[]) {
  char cur;
  char* p = buf;
  //set argv of xargs argvs into argv buffer
  argvBuf[0] = argv[1];
  int preDefinedArgvIdx = 1;  // the command itself
  if (argc > 2) {
    while (preDefinedArgvIdx + 1 < argc) {
      argvBuf[preDefinedArgvIdx] = argv[preDefinedArgvIdx + 1];
      preDefinedArgvIdx++;
    }
  }

  int argvIdx = preDefinedArgvIdx;
  while (read(stdin, &cur, 1) == 1) {
    if (cur != '\n' && cur != ' ') {
      *p++ = cur;
    } else {
      *p = '\0';
      argvBuf[argvIdx] = (char*) malloc(strlen(buf) + 1);
      memcpy(argvBuf[argvIdx++], buf, strlen(buf) + 1);
      p = buf;
      if (cur == '\n') {
        int res = fork();
        if (res < 0) {
          fprintf(stderr, "Fork failed!");
          exit(1);
        }
        if (res == 0) {
          exec(argv[1], argvBuf);
          exit(0);
        }
        //free memory and reset argv
        int resetIdx;
        for (resetIdx = preDefinedArgvIdx; resetIdx < argvIdx - 1; resetIdx++) {
          free(argvBuf[resetIdx]);
        }
        argvIdx = preDefinedArgvIdx;
        wait(0);
      }
    }
  }
  exit(0);
}