#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "err.h"
#include "ins.h"
#include "asm.h"

// this is a wild guess (allows for 512 instructions before reallloc
#define INIT_BUF_SIZE 2048

struct asm_unit {
	// TODO: symbol table trie

	struct gen_ins *ins;
	size_t cap;
	size_t len;
};

static void asmf(struct asm_unit *, FILE *, struct err_set *);

/* Consume the first instruction read into the given gen_ins struct pointer.
 * Errors are set in the given error, and a pointer to the next location is
 * returned. */
static char *read_ins(char *, struct gen_ins *, struct err *);

struct asm_unit *
asm_unit_parse(FILE *restrict f, struct err_set *es)
{
	struct asm_unit *x = NULL;

	x = malloc(sizeof(struct asm_unit));
	x->cap = INIT_BUF_SIZE;
	x->ins = malloc(x->cap);
	x->len = 0;

	asmf(x, f, es);

	fclose(f);
	return x;
}

void
asm_unit_write(FILE *restrict o, const struct asm_unit *restrict u)
{
	struct gen_ins *x;
	for (int i = 0; i < u->len; i++) {
		x = &u->ins[i];
		fwrite(&x->op, 1, 1, o);
		fwrite(&x->reg, 1, 1, o);
		fwrite(&x->imdte, 8, 1, o);
	}
}

void
asm_destroy_unit(struct asm_unit *x)
{
	if (x->ins) free (x->ins);
	free(x);
}

static void
asmf(struct asm_unit *u, FILE *f, struct err_set *es)
{
	struct gen_ins g;
	struct err e;
	char *ln;
	size_t l;

	ln = fgetln(f, &l);
	ln[--l] = '\0'; // null-terminate

	for (;;) {
		if (*ln == '\0') {
			ln = fgetln(f, &l);
			if (ln == NULL) break;
			else ln[--l] = '\0';
		}

		ln = read_ins(ln, &g, &e);
		if (e.type == RE_NOERR)
			// TODO: protect against buffer overflow
			u->ins[u->len++] = g;
		else err_append(es, e);
	}
}

static char *
read_ins(char *in, struct gen_ins *out, struct err *e)
{
	size_t oplen = 0;

	e->type = RE_NOERR;
	out->op = 0;
	out->reg = 0x00;
	out->imdte = 0;

	for (; in[oplen] != '\0' && !isspace(in[oplen]); oplen++);

	if (strncmp(in, "hlt", oplen) == 0) {
		out->op = O_HLT;
		return in + 3;
	} else if (strncmp(in, "nop", oplen) == 0) {
		out->op = O_NOP;
		return in + 3;
	} else {
		e->type = RE_NOINS;
		e->data.ins = strndup(in, oplen);
		return in + oplen;
	}
}
