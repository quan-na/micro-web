/*
  this file show how to create a pipe and pass it
  to a child process.
  the pipes only work uni-directional.
  TODO: pass the pipe to another program.
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char**argv) {
  int pid, n;
  int fd[2];
  pipe(fd);
  char *data = "hello... this is sample data";
  char buf[2015];

  switch (pid = fork()) {
  case 0: /* child */
    close(fd[1]);
    printf(">>> start child process.\n");
    do {
      printf("read %d from pipe\n", n);
    } while ((n = read(fd[0], buf, 1024)) <= 0);
    buf[n] = 0;
    printf("read %d bytes from the pipe: \n%s\n", n, buf);
    printf("<<< end child process.\n");
    return 0;
  default: /* parent */
    close(fd[0]);
    printf(">>> start parent process.\n");
    write(fd[1], data, strlen(data));
    sleep(5);
    printf("<<< end parent process.\n");
    break;
  case -1:
    perror("fork");
    return 1;
  }
  return 0;
}
