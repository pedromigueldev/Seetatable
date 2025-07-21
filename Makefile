all: main

main: main.c table.c
	gcc main.c -o Seetatable -g -Wall
