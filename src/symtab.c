#include <stdlib.h>

#include "symtab.h"

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

	if (x[1] == '\0')
		s->terminal = 1;
	else if (x[0] == s->x)
		s->next = st_append(s->next, x + 1, addr);
	else if (x[0] < s->x)
		s->left = st_append(s->left, x + 1, addr);
	else if (x[1] > s->x)
		s->right = st_append(s->right, x + 1, addr);

	return s;
}
