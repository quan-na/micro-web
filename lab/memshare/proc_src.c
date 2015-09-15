#include <stdlib.h>
#include <stdio.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>

#include <errno.h>

int
main ()
{
  // open share file descriptor
  int sf = shm_open ("/micro-web.test", O_CREAT | O_RDWR, S_IRWXU);
  if (sf == -1)
    {
      printf ("share object open failed\n");
    }

  // fork and exec destination process
  int pid = fork();
  if (-1 == pid)
    {
      printf ("fork failed\n");
      shm_unlink ("/micro-web.test");
      return -1;
    }
  if (0 == pid)
    {
      // child process: call proc_dest
      char fdarg[10];
      sprintf (fdarg, "%d", sf);
      execl ("./proc_dest", fdarg, 0);
      printf ("execl failed\n");
      shm_unlink ("/micro-web.test");
      return 0;
    }
  // main process: map memory and write to it
  char* mem_seg = mmap (NULL, 100, PROT_READ | PROT_WRITE, MAP_SHARED, sf, 0);
  if (mem_seg == MAP_FAILED)
    {
      printf ("main proc memory map failed %d \n", errno);
      return -1;
    }
  mem_seg[0] = 'h';
  mem_seg[1] = 'e';
  mem_seg[2] = 'l';
  mem_seg[3] = 'l';
  mem_seg[4] = 'o';
  mem_seg[5] = ' ';
  mem_seg[6] = 'w';
  mem_seg[7] = 'o';
  mem_seg[8] = 'r';
  mem_seg[9] = 'l';
  mem_seg[10] = 'd';
  mem_seg[11] = '!';
  mem_seg[12] = 0;
  
  // FIXME : remove this
  //shm_unlink ("/micro-web.test");
  return 0;
}
