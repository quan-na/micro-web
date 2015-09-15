#include <stdlib.h>
#include <stdio.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>

int
main (int argc, char** argv)
{
  int sf = atoi(argv[1]);
  //printf("share file handler %d\n", sf);
  char* mem_seg = mmap (NULL, 100, PROT_READ, MAP_SHARED, sf, 0);
  do
    {
      printf ("waiting for shared memory\n");
      sleep(1);
    }
  while (mem_seg[0] != 'H' && mem_seg[12] != 0);
  printf ("proc 2 received %s\n", mem_seg);
  shm_unlink ("/micro-web.test");
  return 0;
}
