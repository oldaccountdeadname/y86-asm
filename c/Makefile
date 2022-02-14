CC=cc
CFLAGS=-std=c17 -Wall -Werror -g

y86-asm: main.o driver.o asm.o err.o
	$(CC) $(CFLAGS) main.o driver.o asm.o err.o -o y86-asm

err.o: err.h err.c
	$(CC) $(CFLAGS) -c err.c -o err.o

asm.o: asm.c asm.h err.h ins.h
	$(CC) $(CFLAGS) -c asm.c -o asm.o

driver.o: driver.h driver.c asm.h err.h
	$(CC) $(CFLAGS) -c driver.c -o driver.o

main.o: main.c driver.h err.h
	$(CC) $(CFLAGS) -c main.c -o main.o

clean:
	rm -f main.o \
	      driver.o \
	      asm.o \
	      y86-asm
