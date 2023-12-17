#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <process.h>

#define MAX_LEN 100

void convertInput(char* input) {
	input[strcspn(input, "\n")] = '\0';
}
char** parseInput(char* input) {
	//Input is not NULL
	//1. Parse input line into words?
	//2. Cmd gets recognized and the arguments will be handled
	//3. Execution depending on the above 

	//ls -l -a --> "ls" -> command, "-l" -> arg1, "-a" -> arg2
	//STEP 1
	int i = 0;
	char** args = (char**) malloc(sizeof(char*)*10);
	char* pch;
	pch = strtok(input, " \t");
	//if input is empty: error
	if (pch == NULL) {
		printf("\nInvalid command.");
		return NULL;
	}
	//args[0] equals command, the rest are the arguments
	args[i] = (char*)malloc(sizeof(char) * 10);
	//TODO: No memory --> fprintf stderr
	strcpy(args[i], pch);
	printf("CMD: \"%s\"", args[i]);

	while (pch != NULL && i < 10) {
		pch = strtok(NULL, " \t");
		if (pch != NULL) {
			i++;
			args[i] = (char*)malloc(sizeof(char) * 10);
			//TODO: No memory --> fprintf stderr
			strcpy(args[i], pch);
			printf("\nARGUMENT %d: \"%s\"", i, args[i]);
		}
		else break;
	}
	return args;
}
int executeCmd(char** cmd) {
	//command gets recognized, then count other args and handle them
	char* cmd_title = _strdup(cmd[0]);
	if (strcmp(cmd_title, "ls") == 0)
		printf("\nCommand to be executed: \"ls\"");
	else if (strcmp(cmd_title, "exit") == 0)
		return 0;
	else printf("\nInvalid command.");

	return 1;
}
int fork() {
	//Forking a new process
}

int main(int argc, char** argv) {
	char input[MAX_LEN];
	char** cmd = NULL;
	int cont = 1;
	while (cont) {
		printf("\nShell>");
		if (fgets(input, MAX_LEN, stdin) != NULL) {
			//Replace \n with \0
			convertInput(input);
			//Parse cmd
			cmd = parseInput(input);
			//TODO: create a child process via fork()
			//Execute cmd
			if(cmd)
				cont = executeCmd(cmd) ? 1 : 0;
		}
		else fprintf(stderr, "\nPlease type in a command.");
	}
	//Free mem
	free(cmd);
	return EXIT_SUCCESS;
}