#include "register.h"

/* A structure storing the general form of most instructions. Control flow
 * instructions, however, (jXX and call) are exceptions to this form.
 *
 * Note that alignment constraints may reorder structure members, and, as such,
 * this may not be written directly to a file. */
struct gen_ins {
	int op:8; /* A one-byte opcode. */
	int rsrc:4; /* A half-byte source register. (rA) */
	int rdst:4; /* A half-byte dest register. (rA) */
	long imdte; /* 8-byte immediate. */
};

enum op {
	O_HLT = 0x00,
	O_NOP = 0x10,
	// ... (TODO)
};
