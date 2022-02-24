// stddef must be included before this header file.
struct err {
	enum {
		RE_NOERR, RE_FNOOPEN, RE_NOINS, RE_NOREG, RE_BADINT,
	} type;
	union {
		const char *path; /* borrowed. */
		char *ins; /* owned. */
		char *reg; /* owned. */
		char *bint; /* owned. */
	} data;
};

struct err_set {
	size_t len;
	size_t cap;
	struct err *e;
};

void err_append(struct err_set *, struct err);
void err_disp(const struct err *);
void err_free_asc(struct err *);
