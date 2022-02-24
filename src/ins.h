/* A structure storing the general form of most instructions. Control flow
 * instructions, however, (jXX and call) are exceptions to this form. */
struct gen_ins {
	unsigned char op; /* A one-byte opcode. */
	unsigned char reg; /* Store both source and dest registers in a single byte. */
	unsigned long imdte; /* 8-byte immediate. */
};

struct ins {
	enum {
		I_GEN,
	} type;
	union {
		struct gen_ins gen;
	} data;
};

enum op {
	O_HLT = 0x00, /* hlt */
	O_NOP = 0x10, /* nop */
	O_RRM = 0x20, /* rrmovq */
	O_IRM = 0x30, /* irmovq */
	O_RMM = 0x40, /* rmmovq */
	O_MRM = 0x50, /* mrmovq */
	// ... (TODO)
};
