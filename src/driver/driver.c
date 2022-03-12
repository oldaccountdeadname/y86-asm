#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../err.h"
#include "../driver.h"
#include "../symtab.h"
#include "../ins.h"
#include "asm.h"

static struct asm_unit **assemble(const struct run_conf *, struct err_set *);
static void link(struct asm_unit **, size_t, const struct symtab *, struct err_set *);
static struct err_set *alloc_err_set(void);
static struct symtab *make_global_st(struct asm_unit **, size_t);

struct err_set *
make(const struct run_conf *c)
{
	struct asm_unit **units;
	struct err_set *es;
	struct symtab *gst;
	struct err e;
	FILE *o;

	es = alloc_err_set();

	o = fopen(c->outputf, "w");
	if (!o) {
		e.type = RE_FNOOPEN;
		e.path = c->outputf;
		err_append(es, e);
	}


	units = assemble(c, es);

	gst = make_global_st(units, c->input_num);

	if (gst)
		link(units, c->input_num, gst, es);

	for (int i = 0; i < c->input_num; i++) {
		if (units[i]) {
			asm_unit_write(o, units[i]);
			free(units[i]->ins);
			free(units[i]);
		}
	}

	fclose(o);
	free(units);
	st_free(gst);
	return es;
}

void
free_err_set(struct err_set *es)
{
	for (size_t i = 0; i < es->len; i++)
		err_free_asc(&es->e[i]);

	free(es->e);
	free(es);
}


static struct asm_unit **
assemble(const struct run_conf *c, struct err_set *es)
{
	FILE *in;
	struct err err;
	struct asm_unit **l;

	l = malloc(sizeof(struct asm_unit *) * c->input_num);
	for (int i = 0; i < c->input_num; i++) {
		in = fopen(c->inputs[i], "r");

		if (!in) {
			err.type = RE_FNOOPEN;

			err.path = c->inputs[i];
			err.ln = 0;

			err_append(es, err);
			l[i] = NULL;
		} else {
			l[i] = asm_unit_parse(in, es, c->inputs[i]);
			fclose(in);
		}
	}

	return l;
}

static void
link(struct asm_unit **units, size_t n, const struct symtab *st, struct err_set *es)
{
	struct asm_unit *u;
	struct ins *ins;
	long adr;
	struct err e;

	e.type = RE_NOLBL;

	for (size_t ui = 0; ui < n; ui++) { // Iterate over units.
		u = units[ui];
		ins = u->ins;
		e.path = u->path;

		for (size_t ii = 0; ii < u->len; ii++) { // Iterate over instructions.
			if (ins[ii].type != I_CTF || !ins[ii].data.ctf.dest.label)
				continue;

			e.ln = ins[ii].ln;

			adr = st_lookup(st, ins[ii].data.ctf.dest.label);

			if (adr >= 0)
				ins[ii].data.ctf.dest.adr = adr;
			else {
				e.data.label = strdup(ins[ii].data.ctf.dest.label);
				err_append(es, e);
			}

			free(ins[ii].data.ctf.dest.label);
			ins[ii].data.ctf.dest.label = NULL;
		}
	}
}

static struct err_set *
alloc_err_set(void)
{
	struct err_set *es = malloc(sizeof(struct err_set));

	es->e = malloc(sizeof(struct err));
	es->len = 0;
	es->cap = 1;

	return es;
}

static struct symtab *
make_global_st(struct asm_unit **units, size_t len)
{
	struct symtab *gst;

	if (!units[0]) return NULL;

	gst = units[0]->st;

	// This looks very loop-unrolling-ammenable: try that if performance
	// ever becomes an issue.
	for (size_t i = 1; i < len; i++)
		st_merge(gst, units[i]->st);

	return gst;
}
