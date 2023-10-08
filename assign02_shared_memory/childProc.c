#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "ipcEx.h"


int main(int argc, char** argv)
{
  char data_string[] = "Hello Shared Memory!\n";


  if(argc != 2) {
      printf("CHILD: There was an issue passing arguments into the child process\n");
      exit(1);
  } else {
      printf("CHILD: Beginning Execution of child process\n");
      struct ipc_struct* shared_memory = (struct ipc_struct*)shmat(atoi(argv[1]), NULL, 0);
      if(shared_memory == (void *)-1) {
          perror("CHILD: shmat error");
          exit(1);
      }


      //I should've calculated sizeof outside of loop for optimization
      for(int i = 0; i < shared_memory->repeat_val; i++) {
          snprintf(shared_memory->data+(i*(sizeof(data_string)-1)),
                   sizeof(shared_memory->data),
                   "%s",
                   data_string);
          //printf("CHILD: In loop\n");
      }

      if ((shmdt(shared_memory)) < 0) {
          perror("CHILD: shmdt error");
      }
  }

  return 0;
}



