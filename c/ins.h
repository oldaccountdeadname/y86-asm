#include "register.h"

/* A structure storing the general form of most instructions. Control flow
 * instructions, however, (jXX and call) are exceptions to this form.
 *
 * The structure directly corresponds to the on-disk format. Definitely makes
 * this not work when aligned access is a CPU requirement, but eh :/ */
struct __attribute__((__packed__)) gen_ins {
	unsigned int op:8; /* A one-byte opcode. */
	unsigned int rsrc:4; /* A half-byte source register. (rA) */
	unsigned int rdst:4; /* A half-byte dest register. (rA) */
	unsigned long imdte; /* 8-byte immediate. */
};

enum op {
	O_HLT = 0x00,
	O_NOP = 0x10,
	// ... (TODO)
};
