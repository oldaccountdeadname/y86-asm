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

	es = alloc_err_set();
	units = assemble(c, es);

	for (int i = 0; i < c->input_num; i++)
		if (units[i]) asm_destroy_unit(units[i]);

	free(units);
	return es;
}

void
free_err_set(struct err_set *es)
{
	free(es->e);
	free(es);
}


static struct asm_unit **
assemble(const struct run_conf *c, struct err_set *es)
{
	struct asm_unit **l = malloc(sizeof(struct asm_unit *) * c->input_num);
	for (int i = 0; i < c->input_num; i++)
		l[i] = asm_unit_parse(c->inputs[i], es);

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