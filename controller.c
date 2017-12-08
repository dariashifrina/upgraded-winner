#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <string.h>
#include <errno.h>

#define KEY 12

void createsem(char *argv[]){
    int id = semget(KEY, 1, IPC_CREAT | IPC_EXCL | 0600);
    if(id < 0){
      printf("Semaphore already exists, please remove using ./a.out -r before tryin again.\n");
    }
    else{
      int newsem = semctl(id,0,SETVAL,atoi(argv[2]));
      printf("Semaphore created! Here it is: %d\n", id);
    }
}

int viewsem(char *argv[]){
  int id = semget(KEY, 1, 0600);
  int semneeded = semctl(id,0,GETVAL);
  if(semneeded == -1){
    printf("Semaphore doesn't exist. Please use ./a.out -c before attempting to view the file.\n");
    return 0;
  }
  else{
    printf("Semaphore %d's value: %d\n", id, semneeded);
    return 1;
  }
}

void removesem(char *argv[]){
  int id = semget(KEY, 1, 0600);
  int semneeded = semctl(id,0,IPC_RMID);
  if(semneeded == -1){
    printf("Semaphore doesn't exist. Please use ./a.out -c before attempting to remove the file.\n");
  }
  else{
    printf("Semaphore is removed: %d\n", semneeded);
  }
}


///////////////////////////////////////////////////////////////

/*int main(int argc, char *argv[]){
  int id;
  if(argc < 2){
    printf("USE ONLY ONE ARGUMENT.\n");
  }
    else{
      if(!strcmp("-c", argv[1])){
	createsem(argv);
    }
    else if(!strcmp("-v", argv[1])){
      viewsem(argv);
    }
      
    else if(!strcmp("-r", argv[1])){
      removesem(argv);
    }
    }
  return 0;
}
*/
