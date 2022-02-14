// stddef and err.h must be included prior to this header file.

struct asm_unit;

struct asm_unit *asm_unit_parse(FILE *, struct err_set *);

void asm_destroy_unit(struct asm_unit *);
