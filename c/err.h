// stddef must be included before this header file.
#ifndef ERR_H
#define ERR_H
struct err {
	enum {
		RE_NOERR, RE_FNOOPEN,
	} type;
	union {
		const char *path;
	} data;
};

struct err_set {
	size_t len;
	size_t cap;
	struct err *e;
};

void err_append(struct err_set *, struct err);
void err_disp(const struct err *);

#endif