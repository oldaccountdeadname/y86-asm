#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "err.h"

void
err_append(struct err_set *es, struct err e)
{
	// XXX: this doesn't handle allocation failures.
	if (es->len >= es->cap) {
		// Roughly double the size. Adding 1 to the initial size lets
		// this work when es->cap == 0.
		es->cap = (es->cap + 1) * 2;
		es->e = realloc(es->e, es->cap * sizeof(struct err));
	}

	es->e[es->len++] = e;
}

void
err_disp(const struct err *e)
{
	fprintf(stderr, "\033[1;31mError:\033[0m ");
	switch (e->type) {
	case RE_FNOOPEN:
		fprintf(stderr, "Couldn't open file %s.\033[0m\n", e->data.path);
		fprintf(stderr, "       \033[0;33mDoes it exist with proper permissions?\n");
		break;
	default:
		break;
	}

	fprintf(stderr, "\033[0m");
}
