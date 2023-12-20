#include "libs.h"
#include "constants.h"
#include "structs.h"
#include "executables.h"

int in_range(int ref, int min, int max) {
	return (ref >= min && ref <= max) ? 1 : 0;
}

void convertInput(char* input) {
	input[strcspn(input, "\n")] = '\0';
}
char** parseInput(char* input, int* n_args) {
	//Input is not NULL
	//1. Parse input line into words depending on white spaces
	//2. Cmd gets recognized in the exec func

	int i = 0;
	char** args = (char**) malloc(sizeof(char*)*10);
	if (args == NULL) {
		fprintf(stderr, "\nFatal memory error.");
		exit(EXIT_FAILURE);
	}
	char* pch;
	pch = strtok(input, " \t");
	//if input is empty: error
	if (pch == NULL) {
		printf("\nInvalid command.");
		return NULL;
	}
	//args[0] equals command, the rest are the arguments
	args[i] = (char*)malloc(sizeof(char) * 10);
	if (args == NULL) {
		fprintf(stderr, "\nFatal memory error.");
		exit(EXIT_FAILURE);
	}
	(*n_args)++;
	strcpy(args[i], pch);

	while (pch != NULL && i < MAX_ARGS) {
		pch = strtok(NULL, " \t");
		if (pch != NULL) {
			i++;
			args[i] = (char*)malloc(sizeof(char) * 10);
			if (args == NULL) {
				fprintf(stderr, "\nFatal memory error.");
				exit(EXIT_FAILURE);
			}
			(*n_args)++;
			strcpy(args[i], pch);
		}
		else break;
	}
	return args;
}

int executeCmd(Command* cmd) {
	//->Command gets recognized and the corresponding function gets called
	char* cmd_title = _strdup(cmd->args[0]);
	for (int i = 0; i < CMD_LIST; i++) {
		if (strcmp(ExecTable[i].cmd, cmd_title) == 0 
			&& in_range(cmd->n_args, ExecTable[i].min_args, ExecTable[i].max_args)) 
		{
			ExecTable[i].handler(cmd);
			return 1;
		}
	}
	printf("\nError: Command not found or invalid number of arguments. Type \"help\" for assistance.");
	return 1;
}

int main(int argc, char** argv) {
	char input[MAX_LEN];

	Command cmd;
	cmd.args = NULL;

	int cont = 1;

	while (cont) {
		char cwd[MAX_LEN];
		if (_getcwd(cwd, sizeof(cwd)) == NULL) {
			fprintf(stderr, "Directory could not be found.");
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