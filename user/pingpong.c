#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  int child_to_parent[2];
  int parent_to_child[2];
  pipe(child_to_parent);
  pipe(parent_to_child);
  if (fork() == 0) {
    close(parent_to_child[1]);
    close(child_to_parent[0]);
    char b;
    read(parent_to_child[0], &b, 1);
    fprintf(1, "%d: received ping\n", getpid());
    write(child_to_parent[1], &b, 1);
    close(parent_to_child[0]);
    close(child_to_parent[1]);
  } else {
    char send = 'a';
    close(parent_to_child[0]);
    close(child_to_parent[1]);
    write(parent_to_child[1], &send, 1);
    read(child_to_parent[0], &send, 1);
    fprintf(1, "%d: received pong\n", getpid());
    close(parent_to_child[1]);
    close(child_to_parent[0]);
  }
  exit(0);
}