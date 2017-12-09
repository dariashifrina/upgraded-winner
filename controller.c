#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define KEY 12

//doesnt work without manual declaration for some reason. say storage size su unknown
union semun {
  int val;
  struct semid_ds *buf;
  unsigned short int *array;
  struct seminfo *__buf;
};

void createsem(){
  int shmid = shmget(KEY, 1024, IPC_CREAT | IPC_EXCL | 0644); //shared mem
    if(shmid < 0){
      printf("Semaphore already exists, please remove using ./a.out -r before tryin again.\n");
    }
    else{
      int newsem = open("story.txt", O_CREAT | O_TRUNC, 0644); //creating story file
      int semid = semget(KEY, 1, IPC_CREAT | IPC_EXCL | 0644); //getting sem id
      printf("Semaphore created! Here it is: %d\n", semid);
      union semun su;
      su.val = 1;
      semctl(semid,0,SETVAL, su); //storing metadata
    }
}

void viewsem(){ //view current story
  int shmid = shmget(KEY, 1024, 0644); //shared mem
  // int semneeded = semctl(id,0,GETVAL);
  if(shmid == -1){
    printf("Semaphore doesn't exist. Please use ./a.out -c before attempting to view the file.\n");
  }
  else{
    char * story = (char *)calloc(sizeof(char),1024);
    int fd = open("story.txt",O_RDONLY, 0644);
    read(fd,story,1024);
    printf("Story so far: %s\n",story);
  }
}

void removesem(){
  int shmid = shmget(KEY, 1024, 0); //shared mem
  //int semneeded = semctl(id,0,IPC_RMID);
  if(shmid == -1){
    printf("Semaphore doesn't exist. Please use ./a.out -c before attempting to remove the file.\n");
  }
  else{
    int semid = semget(KEY, 1, 0);
    char * story = (char *)calloc(sizeof(char),1024);
    int fd = open("story.txt",O_RDONLY, 0644);
    read(fd,story,1024);
    printf("Story is being removed: %s\n",story); 
    shmctl(shmid,IPC_RMID,0); //marking segment to be destroyed and destroys it
    semctl(semid,0,IPC_RMID,0); //removing semaphore indetifier and freeing storage
    remove("story.txt"); //story removed
  }
}


///////////////////////////////////////////////////////////////

int main(int argc, char *argv[]){
  int id;
  if(argc < 2){
    printf("USE ONLY ONE ARGUMENT.\n");
  }
    else{
      if(!strcmp("-c", argv[1])){
	createsem();
    }
    else if(!strcmp("-v", argv[1])){
      viewsem();
    }
      
    else if(!strcmp("-r", argv[1])){
      removesem();
    }
    }
  return 0;
}

