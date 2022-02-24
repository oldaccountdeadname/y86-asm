struct dest {
	const char *label; /* May be NULL. */
	unsigned long adr; /* Must be used if label is NULL. */
};

/* A structure storing the general form of most instructions. Control flow
 * instructions, however, (jXX and call) are exceptions to this form. */
struct gen_ins {
	unsigned char op; /* A one-byte opcode. */
	unsigned char reg; /* Store both source and dest registers in a single byte. */
	unsigned long imdte; /* 8-byte immediate. */
};

/* A structure storing metadata of control flow instructions jXX and call. */
struct ctf_ins {
	unsigned char op;
	struct dest dest;
};

struct ins {
	enum {
		I_GEN, I_CTF,
	} type;
	union {
		struct gen_ins gen;
		struct ctf_ins ctf;
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
	O_JMP = 0x70,
	// ... (TODO)
};

/* Condition encodings used in jXX and cmovXX instructions. */
enum cond {
	C_UNCOND = 0x00,
	C_LE     = 0x01,
	C_L      = 0x02,
	C_E      = 0x03,
	C_NE     = 0x04,
	C_GE     = 0x05,
	C_G      = 0x06,
};
