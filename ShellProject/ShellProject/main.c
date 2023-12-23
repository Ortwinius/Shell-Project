#include "libs.h"
#include "constants.h"
#include "structs.h"
#include "executables.h"
#include "parser.h"

int main(int argc, char** argv) {
	char input[MAX_LEN];

	Command cmd;
	cmd.args = NULL;

	int cont = 1;

	while (cont) {
		char cwd[MAX_LEN];
		if (_getcwd(cwd, sizeof(cwd)) == NULL) {
			fprintf(stderr, "Fatal error: directory could not be found.");
			exit(EXIT_FAILURE);
		}
		printf("\n%s>", cwd);
		if (fgets(input, MAX_LEN, stdin) != NULL) {
			//Replace \n with \0
			convertInput(input);
			//Reset n_args
			cmd.n_args = 0;
			//Parse cmd
			cmd.args = parseInput(input, &cmd.n_args);
			//Execute cmd
			if(cmd.args)
				//ternary not necessary
				cont = executeCmd(&cmd) ? 1 : 0;
		}
		else fprintf(stderr, "\nPlease type in a command.");
	}
	//Free mem
	free(cmd.args);
	return EXIT_SUCCESS;
}