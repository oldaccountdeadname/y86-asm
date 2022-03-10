#define EMPTYSYMBOL(s) \
	s->left = s->right = s->next = NULL; \
	s->terminal = 0; \
	s->x = '\0';
	// Note that we don't bother setting addr, as that only matters when
	// terminal is 1.

struct symtab {
	struct symtab *left;
	struct symtab *right;
	struct symtab *next;

	long addr;
	int terminal;
	char x;
};

struct symtab *st_append(struct symtab *, const char *, long addr);
