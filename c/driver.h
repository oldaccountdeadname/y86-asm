// err.h must be included before this header file.

struct run_conf {
	char *outputf;
	char **inputs;
	int input_num;
};

struct err_set *make(const struct run_conf *);
void free_err_set(struct err_set *);
