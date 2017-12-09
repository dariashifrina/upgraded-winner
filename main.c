//main file

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include <fcntl.h>

#define KEY 12 //decided not to use for shmget only using for semget bc can't access the same semaphore from 2 files!!


//buffer for reading user input into a string
void bufferz( char * buff ){
  char * buffer = (char *)calloc(1024,sizeof(char));
  fgets(buffer, 1024, stdin);
  // printf("buffer is: %s\n", buff);
  int i = 0;
  while(buffer[i] != '\0'){
    //if(strcmp(buffer[i], "/n") == 0){
    if(buffer[i]== '\n'){
      buffer[i] = 0;
    }
    else{
      i++;
    }
  }
strncpy(buff, buffer, 1024);
free(buffer);
}


//prints last line from story
void lastline(int * seg){
  int fd = open("story.txt",O_RDWR | O_APPEND, 0644);
  char * last_line = (char *)calloc(sizeof(char),1024);
  lseek(fd,-(*seg),SEEK_END);
  read(fd,last_line,*seg);
  printf("Last line of story: %s\n", last_line);
}

//gets message from user input and writes to story
void getmessage(int * seg){
  int fd = open("story.txt",O_RDWR | O_APPEND, 0644);
  char buffer[1024];
  char *p = buffer;
  bufferz(buffer);
  //printf("%s\n", p);
  write(fd, buffer, strlen(buffer));
  *seg = strlen(buffer);
}



int main(){
  //setting up mem sharing and semaphores
  int semid = semget(KEY, 1, 0644); //creating or accessing semaphore
  struct sembuf semba;
  semba.sem_num = 0;
  semba.sem_flg = SEM_UNDO;
  semba.sem_op = -1;
  semop(semid,&semba,1); //decrementing the value of semaphore upon exit
  int shmid = shmget(ftok("makefile", 13), 1024, 0644); //creating universal key to use
  //int fd = open("story.txt",O_RDWR | O_APPEND, 0644);

  
  if(shmid == -1){
    printf("You need to create a semaphore first!!! Run ./controller -c before trying again.\n");
  }
  else{
    int *segment = (int *) shmat(shmid,0,0);
    
    if(*segment != 0){
      lastline(segment);
    }
    
    printf("Enter a new sentence to add:\n");
 
    getmessage(segment);
    semba.sem_op = 1; //incrementing value of semaphore back to original so it can be used again!!!
    semop(semid, &semba, 1);
    return 0;
  }
}
