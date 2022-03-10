// stddef and err.h must be included prior to this header file.

struct asm_unit {
	struct symtab *st;

	struct ins *ins;
	size_t cap;
	size_t len;
};

/* Parse a given FILE, recording errors in the given err_set, with the given
 * string borrowed as the filename. (The given string must live as long as the
 * error set.) */
struct asm_unit *asm_unit_parse(FILE *, struct err_set *, const char *);
void asm_unit_write(FILE *restrict, const struct asm_unit *restrict);
