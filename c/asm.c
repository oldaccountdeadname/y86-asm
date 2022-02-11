#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "err.h"
#include "asm.h"

struct asm_unit {
	// TODO: symbol table trie
	char *bin;
};

struct asm_unit *
asm_unit_parse(const char *p, struct err_set *es)
{
	struct asm_unit *x = NULL;
	struct err err;
	FILE *f;

	f = fopen(p, "r");
	if (!f) {
		err.type = RE_FNOOPEN;
		err.data.path = p;
		err_append(es, err);
		return NULL;
	}

	x = malloc(sizeof(struct asm_unit));

	fclose(f);
	return x;
}

void
asm_destroy_unit(struct asm_unit *x)
{
	free(x);
}
