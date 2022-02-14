#include "register.h"

/* A structure storing the general form of most instructions. Control flow
 * instructions, however, (jXX and call) are exceptions to this form. */
struct gen_ins {
	unsigned char op; /* A one-byte opcode. */
	unsigned char reg; /* Store both source and dest registers in a single byte. */
	unsigned long imdte; /* 8-byte immediate. */
};

enum op {
	O_HLT = 0x00,
	O_NOP = 0x10,
	// ... (TODO)
};
