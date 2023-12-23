#include "libs.h"
#include "constants.h"
#include "parser.h"

void convertInput(char* input) {
	input[strcspn(input, "\n")] = '\0';
}
char** parseInput(char* input, int* n_args) {
	//Input is not NULL
	//1. Parse input line into words depending on white spaces
	//2. Cmd gets recognized in the exec func

	int i = 0;
	char** args = (char**)malloc(sizeof(char*) * 10);
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