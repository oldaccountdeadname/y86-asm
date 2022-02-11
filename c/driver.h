struct run_conf {
	char *outputf;
	char **inputs;
	int input_num;
};

int assemble(const struct run_conf *);
