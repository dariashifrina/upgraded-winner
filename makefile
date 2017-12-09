all: compile1 compile2

compile1: controller.c 
	gcc -g controller.c -o controller

compile2: main.c
	gcc -g main.c -o main

clean:
	rm controller
	rm main
	rm story.txt
