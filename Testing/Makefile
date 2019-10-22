# choose your compiler
CC=gcc
#CC=gcc -Wall

test: myTest.o
	$(CC) -g myTest.o -o test

myTest.o: myTest.c
	$(CC) -g -c myTest.c