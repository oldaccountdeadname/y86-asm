// stddef and err.h must be included prior to this header file.

struct asm_unit;

struct asm_unit *asm_unit_parse(FILE *, struct err_set *);
void asm_unit_write(FILE *restrict, const struct asm_unit *restrict);

void asm_destroy_unit(struct asm_unit *);