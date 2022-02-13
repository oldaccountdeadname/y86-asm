#include <stdio.h>
#include <stdbool.h>
#include <memory.h>
#include "memory.h"
#include "tools.h"

static int memory[MEMSIZE];

void
clear_memory(void) {
	memset(memory, 0, MEMSIZE * sizeof(*memory));
}

void
store(int address, int value, bool *memError)
{
	if (address < 0 || address > MEMSIZE) {
		(*memError) = true;
	} else {
		(*memError) = false;
		memory[address] = value;
	}
}

int
fetch(int address, bool * memError)
{
	if (address < 0 || address > MEMSIZE) {
		(*memError) = true;
	} else {
		(*memError) = false;
		return memory[address];
	}
}

char
get_byte(int address, bool *memError)
{
	if (address < 0 || address > MEMBYTE) {
		(*memError) = true;
		return 0;
	} else {
		(*memError) = false;
		int w_addr = fetch((address / 4), memError);
		char *addr = (char*)&w_addr;
		char retn = addr[(address % 4)];
		return retn;
	}
}

void
set_byte(int address, char value, bool * memError)
{
	if (address < 0 || address > MEMBYTE) {
		(*memError) = true;
	} else {
		(*memError) = false;
		int w_addr = fetch((address / 4), memError);
		char * addr = (char*)&w_addr;
		addr[(address % 4)] = value;
		store(address / 4, w_addr, memError);
	}
}

int
get_word(int address, bool *memError)
{
	if (address < 0 || address > MEMBYTE || (address % 4) != 0) {
		(*memError) = true;
		return 0;
	} else {
		(*memError) = false;
		return fetch(address / 4, memError);
	}
}

void
set_word(int address, int value, bool *memError)
{
	if (address < 0 || address > MEMBYTE || (address % 4) != 0) {
		(*memError) = true;
	} else {
		(*memError) = false;
		store((address / 4), value, memError);
	}
}
