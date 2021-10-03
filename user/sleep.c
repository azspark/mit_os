#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  if(argc < 2) {
    fprintf(2, "Usage: sleep with what times...\n");
    exit(1);
  }
  int i;
  for (i = 1; i < argc; i++) {
    int ticks = atoi(argv[i]);
    sleep(ticks);
  }
  exit(0);
}