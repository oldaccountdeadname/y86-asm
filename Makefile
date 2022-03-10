CC?=cc
CFLAGS+=-D_POSIX_C_SOURCE=200809L -std=c99 -Wall -Wextra -Werror -g

y86-asm: src/main.o src/driver/driver.o src/driver/asm.o src/err.o src/symtab.o
	$(CC) $(CFLAGS) src/main.o src/driver/driver.o src/driver/asm.o \
		src/err.o src/symtab.o \
		-o y86-asm

src/err.o: src/err.h src/err.c
src/driver/asm.o: src/driver/asm.c src/driver/asm.h src/err.h src/ins.h src/symtab.h
src/driver/driver.o: src/driver.h src/driver/driver.c src/driver/asm.h src/err.h
src/main.o: src/main.c src/driver.h src/err.h
src/symtab.o: src/symtab.h src/symtab.c

clean:
	rm -f src/main.o \
	      src/err.o \
	      src/driver/driver.o \
	      src/driver/asm.o \
	      y86-asm
