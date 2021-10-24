#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

#define stdin 0
#define stdout 1
#define stderr 2

char *fmtname(char *path) {
  char *p;
  for(p = path + strlen(path); p >= path && *p != '/'; --p);
  return p+1;
}

void findFile(char* path, char* target) {
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;
  // printf(path);

  if((fd = open(path, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }
  
  if (st.type == T_FILE) {
    if (strcmp(fmtname(path), target) == 0) {
      printf("%s\n", path);
    }
  } else if (st.type == T_DIR) {
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum == 0)
          continue;
        if (!strcmp(de.name, ".") || !strcmp(de.name, "..")) {
          continue;
        }
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        findFile(buf, target);
        // printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
    }
  }
  close(fd);
}

int main(int argc, char *argv[]) {

  if (argc != 3) {
    fprintf(stdout, "What are you find for?");
    exit(0);
  }
  findFile(argv[1], argv[2]);
  exit(0);
}