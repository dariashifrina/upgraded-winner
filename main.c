//main file

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "controller.c"

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

void lastline(){
  int fd;
  fd=open("story.txt",O_RDONLY, 0666);
  char *file_contents = (sizeof(char));
  fread(file_contents, sizeof(char), 423423, fd);
  printf("%s", file_contents);
  printf("Something happened somewhere.\n");
}

void run(){
  char buffer[1024];
  char *p = buffer;
  bufferz(buffer);
  printf("%s\n", p);
}

int main(){
  printf("Last line in the story:\n");
  lastline();
  printf("Enter a new sentence to add:\n");
  run();
  return 0;
}
