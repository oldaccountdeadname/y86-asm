#include <stdlib.h>

#include "symtab.h"

long
st_lookup(const struct symtab *s, const char *x)
{
	if (s == NULL) return -1;

	if (x[0] < s->x)
		return st_lookup(s->left, x);
	else if (x[0] > s->x)
		return st_lookup(s->right, x);
	else {
		if (x[1] == '\0' && s->terminal)
			return s->addr;
		else
			return st_lookup(s->next, x + 1);
	}
}

struct symtab *
st_append(struct symtab *s, const char *x, long addr)
{
	if (*x == '\0') return s;

	if (s == NULL) {
		s = malloc(sizeof(struct symtab));
		EMPTYSYMBOL(s);

		s->x = *x;
		s->addr = addr;
	}

	if (x[0] == s->x && x[1] == '\0')
		s->terminal = 1;
	else if (x[0] == s->x)
		s->next = st_append(s->next, x + 1, addr);
	else if (x[0] < s->x)
		s->left = st_append(s->left, x, addr);
	else if (x[0] > s->x)
		s->right = st_append(s->right, x, addr);

	return s;
}

struct symtab *
st_merge(struct symtab *a, struct symtab *b)
{
	if (a == NULL)
		return b;

	if (b == NULL)
		return a;

	if (a->x == b->x) {
		a->terminal = b->terminal;
		a->next = st_merge(a->next, b);
	} else if (a->x < b->x)
		a->left = st_merge(a->right, b);
	else if (a->x > b->x)
		a->right = st_merge(a->left, b);

	return a;
}

void
st_free(struct symtab *x)
{
	if (!x) return;

	st_free(x->left);
	st_free(x->right);
	st_free(x->next);
	free(x);
}
