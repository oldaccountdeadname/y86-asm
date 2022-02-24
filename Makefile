CC=cc
CFLAGS=-D_POSIX_C_SOURCE=200809L -std=c17 -Wall -Werror -g

y86-asm: src/main.o src/driver.o src/asm.o src/err.o
	$(CC) $(CFLAGS) src/main.o src/driver.o src/asm.o src/err.o -o y86-asm

src/err.o: src/err.h src/err.c
	$(CC) $(CFLAGS) -c src/err.c -o src/err.o

src/asm.o: src/asm.c src/asm.h src/err.h src/ins.h
	$(CC) $(CFLAGS) -c src/asm.c -o src/asm.o

src/driver.o: src/driver.h src/driver.c src/asm.h src/err.h
	$(CC) $(CFLAGS) -c src/driver.c -o src/driver.o

src/main.o: src/main.c src/driver.h src/err.h
	$(CC) $(CFLAGS) -c src/main.c -o src/main.o

clean:
	rm -f src/main.o \
	      src/driver.o \
	      src/asm.o \
	      src/err.o \
	      y86-asm
