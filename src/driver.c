#include <stdio.h>
#include <stdlib.h>

#include "err.h"
#include "driver.h"
#include "asm.h"

static struct asm_unit **assemble(const struct run_conf *, struct err_set *);
static struct err_set *alloc_err_set(void);

struct err_set *
make(const struct run_conf *c)
{
	struct asm_unit **units;
	struct err_set *es;
	struct err e;
	FILE *o;

	es = alloc_err_set();

	o = fopen(c->outputf, "w");
	if (!o) {
		e.type = RE_FNOOPEN;
		e.data.path = c->outputf;
		err_append(es, e);
	}
	units = assemble(c, es);

	for (int i = 0; i < c->input_num; i++) {
		if (units[i]) {
			asm_unit_write(o, units[i]);
			asm_destroy_unit(units[i]);
		}
	}

	fclose(o);
	free(units);
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
			err.data.path = c->inputs[i];
			err_append(es, err);
			l[i] = NULL;
		} else {
			l[i] = asm_unit_parse(in, es);
			fclose(in);
		}
	}

	return l;
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
