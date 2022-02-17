#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "err.h"
#include "asm.h"
#include "ins.h"
#include "register.h"


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
 * Errors are added to given error set. If no errors occured, 0 is returned,
 * otherwise, a non-zero value is returned. */
static int read_ins(char *, struct gen_ins *, struct err_set *);

static char *read_reg(char *, unsigned char *, struct err_set *);

static char *consume_whitespace(char *);
static void strip_comment(char *);

struct asm_unit *
asm_unit_parse(FILE *restrict f, struct err_set *es)
{
	struct asm_unit *x = NULL;

	x = malloc(sizeof(struct asm_unit));
	x->cap = INIT_BUF_SIZE;
	x->ins = malloc(x->cap);
	x->len = 0;

	asmf(x, f, es);

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
	char *ln;
	ssize_t l;
	size_t c;

	c = l = 0;
	ln = NULL;

	while ((l = getline(&ln, &c, f)) > 0) {
		ln[--l] = '\0'; // null-terminate where newline is
		if (ln[0] == '\0') continue;

		if (read_ins(ln, &g, es) == 0)
			// TODO: protect against buffer overflow
			u->ins[u->len++] = g;
	}

	if (ln) free(ln);
}

static int
read_ins(char *in, struct gen_ins *out, struct err_set *es)
{
	size_t oplen = 0;
	struct err e;

	e.type = RE_NOERR;
	out->op = 0;
	out->reg = 0x00;
	out->imdte = 0;

	in = consume_whitespace(in);
	strip_comment(in);
	for (; in[oplen] != '\0' && !isspace(in[oplen]); oplen++);

	if (strncmp(in, "hlt", oplen) == 0) {
		out->op = O_HLT;
	} else if (strncmp(in, "nop", oplen) == 0) {
		out->op = O_NOP;
	} else if (strncmp(in, "rrmovq", oplen) == 0) {
		out->op = O_RRM;
		in = read_reg(in + 6, &out->reg, es);
		out->reg <<= 4;
		read_reg(in, &out->reg, es);
	} else {
		e.type = RE_NOINS;
		e.data.ins = strndup(in, oplen);
		err_append(es, e);
		return 1;
	}

	return 0;
}

static char *
read_reg(char *in, unsigned char *r, struct err_set *es)
{
	struct err e;
	size_t oplen, padding;
	in = consume_whitespace(in);

	oplen = padding = 0;
	for (; in[oplen] != '\0' && !isspace(in[oplen]); oplen++);

	// strip a trailing comma if present
	if (oplen > 1 && in[oplen - 1] == ',') {
		oplen--;
		padding++;
	}

	if (strncmp(in, "%rax", oplen) == 0) {
		*r |= RAX;
	} else if (strncmp(in, "%rcx", oplen) == 0) {
		*r |= RCX;
	} else if (strncmp(in, "%rdx", oplen) == 0) {
		*r |= RDX;
	} else if (strncmp(in, "%rbx", oplen) == 0) {
		*r |= RBX;
	} else if (strncmp(in, "%rsp", oplen) == 0) {
		*r |= RSP;
	} else if (strncmp(in, "%rbp", oplen) == 0) {
		*r |= RBP;
	} else if (strncmp(in, "%rsi", oplen) == 0) {
		*r |= RSI;
	} else if (strncmp(in, "%rdi", oplen) == 0) {
		*r |= RDI;
	} else if (strncmp(in, "%r8", oplen) == 0) {
		*r |= R8;
	} else if (strncmp(in, "%r9", oplen) == 0) {
		*r |= R9;
	} else if (strncmp(in, "%r10", oplen) == 0) {
		*r |= R10;
	} else if (strncmp(in, "%r11", oplen) == 0) {
		*r |= R11;
	} else if (strncmp(in, "%r12", oplen) == 0) {
		*r |= R12;
	} else if (strncmp(in, "%r13", oplen) == 0) {
		*r |= R13;
	} else if (strncmp(in, "%r14", oplen) == 0) {
		*r |= R14;
	} else {
		e.type = RE_NOREG;
		e.data.reg = strndup(in, oplen);
		err_append(es, e);
	}

	return in + oplen + padding;
}

static char *
consume_whitespace(char *x)
{
	while (*x != '\0' && isspace(*x)) x++;
	return x;
}

static void
strip_comment(char *x)
{
	for (char *c = x; *c != '\0'; c++)
		if (*c == '#') *c = '\0';
}
