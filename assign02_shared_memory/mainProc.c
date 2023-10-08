#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "ipcEx.h"

//Theodore Bloomquist
// 2023



int repVal;
int memID;
int pid;

int main(int argc, char** argv)
{

  /* Print out usage statement when no value is specified */
  if (argc != 2) {
    printf("You must specify an integer value as an argument\n");
    return 1;
  } else {

  }

    //argv[1] contains number of times the child process is required to write string to shared buffer

    //converting argument passed in call
    //to integer for future use
    if((repVal = atoi(argv[1])) == 0) {
        printf("Incorrect Input\n");
        exit(1);
    }

    //creating shared memory space
    if((memID = shmget(IPC_PRIVATE, sizeof(struct ipc_struct), S_IRUSR | S_IWUSR)) == -1) {
        printf("Error Requesting Chunk of Memory\n");
        exit(1);
    }

    printf("PARENT: Memory Space Created! ID = %i\n", memID);

    struct ipc_struct* shared_memory = (struct ipc_struct*)shmat(memID, NULL, 0);
    if(shared_memory == (void *)-1) {
        perror("PARENT: shmat error");
        exit(1);
    }

    printf("PARENT: Successfully Attached to Memory Space\n");

    shared_memory->repeat_val = repVal;

    printf("PARENT: The child process should store its string in shared memory %i times\n", repVal);

    printf("PARENT: Entering Child Process\n");
    pid = fork();

    if(pid < 0) {
        perror("PARENT: Fork error");
        exit(1);
    } else if(pid == 0) {
        //child process
        printf("CHILD: Entered Child Process\n");
        char str[200];
        sprintf(str, "%d", memID);
        if((execlp("./childProc", "childProc", str, NULL)) < 0) {
            perror("PARENT: execlp issue");
            exit(1);
        }
    } else {
        //parent process
        wait(NULL);
        printf("PARENT: Child Process with PID=%i Complete\n", pid);
        printf("PARENT: Child process altered data buffer to be the following:\n");
        printf("----Buffer Start-----\n");
        printf("%s", shared_memory->data);
        printf("-----Buffer End------\n");
        if ((shmdt(shared_memory)) < 0) {
            perror("PARENT: shmdt error");
        }
        printf("PARENT: Successfully detached from shared memory\n");
        if((shmctl(memID, IPC_RMID, NULL)) < 0) {
            perror("PARENT: shmctl error");
        }
        printf("PARENT: Successfully deleted shared memory\n");
    }
  printf("PARENT: Exiting Parent Process\n");
  return 0;
}

