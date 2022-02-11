CC=cc
CFLAGS=-std=c17 -Wall -Werror -g

y86-asm: main.o driver.o
	$(CC) $(CFLAGS) main.o driver.o -o y86-asm

driver: driver.h driver.c
	$(CC) $(CFLAGS) -c driver.c -o driver.o

main.o: main.c driver.h
	$(CC) $(CFLAGS) -c main.c -o main.o

clean:
	rm -f main.o \
	      driver.o \
	      y86-asm
