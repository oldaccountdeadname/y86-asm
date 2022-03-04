#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "err.h"
#include "driver.h"

static void help(int);

/* Parse arguments into given inputs. Returns -1 if parsing has falied, or the
 * number of input filenames parsed otherwise. */
static int parse_args(
	int argc, char **argv, char **inputs, char **output, int max_infs
);

int
main(int argc, char **argv)
{
	struct run_conf rc;
	struct err_set *es;
	int input_num, err_count;

	char *inputs[128];
	char *output = "a.out";

	input_num = parse_args(argc, argv, inputs, &output, 128);
	if (input_num < 0) help(1);

	rc.inputs = inputs;
	rc.outputf = output;
	rc.input_num = input_num;

	es = make(&rc);
	err_count = es->len;

	for (int i = 0; i < err_count; i++)
		err_disp(&es->e[i]);

	free_err_set(es);
	return err_count != 0;
}

static void
help(int c)
{
	printf(
		"    y86-asm [--help] | [-o|--output] <filename> [input] [input] [input]...\n"
		"\n"
		"y86-asm assembles and links multiple y86 assembly plain text files. Pass\n"
		"filenames as arguments to the CLI. If no output filename is specified with the\n"
		"--output or -o flag, y86-asm writes to a.out.\n"
		"\n"
		"A maximum of 128 input paths may be provided.\n"
	);
	exit(c);
}

static int
parse_args(int argc, char **argv, char **inputs, char **output, int max_infs)
{
	char *arg;
	int on_out, next_in;
	on_out = next_in = 0;

	// start iterating at 1 to skip over y86's name
	for (int i = 1; i < argc; i++) {
		arg = argv[i];
		if (on_out) {
			*output = arg;
			on_out = 0;
		} else if (strcmp(arg, "--output") == 0) {
			on_out = 1;
		} else if (strcmp(arg, "--help") == 0) {
			return -1;
		} else if (next_in < max_infs)
			inputs[next_in++] = arg;
	}

	return argc < 2 ? -1 : next_in;
}
