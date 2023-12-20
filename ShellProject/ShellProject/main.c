#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <sys/stat.h>
#include <process.h>
#include <wchar.h>
#include <tchar.h>
#include <direct.h>
#include <minwinbase.h>

#define MAX_LEN 256
#define MAX_ARGS 10
#define CMD_LIST 8

typedef struct {
	const char* cmd;
	const unsigned int min_args;
	const unsigned int max_args;
	void (*handler)(void);
}Exec;

typedef struct {
	int n_args;
	char** args;
}Command;

void c_echo(Command* cmd);
void c_cat(Command* cmd);
void c_cd(Command* cmd);
void c_touch(Command* cmd);
void c_exit(void);
void c_help(void);
char* c_cwd(void);
void c_ls(void);
unsigned int getFileSize(const char* searchpath, WCHAR* filename);

const Exec ExecTable[] = {
	{"exit", 1, 1, c_exit},
	{"help", 1, 3, c_help},
	{"cwd", 1, 1, c_cwd},
	{"ls", 1, 4, c_ls},
	{"cd", 2, 2, c_cd},
	{"echo", 2, MAX_LEN, c_echo},
	{"touch", 2, 3, c_touch},
	{"cat", 2, 3, c_cat},
};

int in_range(int ref, int min, int max) {
	return (ref >= min && ref <= max) ? 1 : 0;
}
void c_exit(void) {
	printf("\nfunc Call: Execute \"exit\"");
	exit(EXIT_SUCCESS);
}
void c_help(void) {
	for (int i = 0; i < CMD_LIST; i++) {
		char cmd_name[MAX_LEN];
		strcpy(cmd_name, ExecTable[i].cmd);
		printf("%-20s %s\n", _strupr(cmd_name), "Description following"); //Cmd Name in upper cases
	}
}
void c_echo(Command* cmd) {
	for (int i = 1; i < cmd->n_args; i++)
	{
		printf("%s ", cmd->args[i]);
	}
}
char* c_cwd(void) {
	char cwd[MAX_LEN];
	if (_getcwd(cwd, sizeof(cwd)) != NULL) {
		printf("Current directory: %s", cwd);
	}
	return cwd;
}
unsigned int getFileSize(const char* searchpath, WCHAR* filename) {
	int file_size = 0;
	struct stat st;

	//Path
	WCHAR path[MAX_LEN];
	wcscpy(path, L"test_files\\"); //TODO: test_files mit searchpath ohne "/*.txt" ersetzen
	wcscat(path, filename);

	//Convert wide string to narrow string
	char narrowPath[MAX_LEN];
	wcstombs(narrowPath, path, sizeof(narrowPath));

	//If file size can be evaluated return it, else err
	if (stat(narrowPath, &st) == 0) {
		file_size = st.st_size;
	}
	else {
		printf("\n[Error %d] ", GetLastError());
		printf("Size (?):", path);
	}
	return file_size;
}
void c_ls(void) {
	const TCHAR* searchpath = _T("test_files\\*.txt");
	HANDLE hFind;
	WIN32_FIND_DATA f;
	hFind = FindFirstFile(searchpath, &f);
	char file_n_buffer[MAX_LEN];

	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			sprintf(file_n_buffer, "%ls", f.cFileName);
			printf("\n%-10d", getFileSize(searchpath, f.cFileName));
			_tprintf(_T("%s"), (f.cFileName));
		} while (FindNextFile(hFind, &f) != 0);
	}
	else {
		DWORD err_code = GetLastError();
		char* err_msg = (err_code == 2) ? "File not found" : "(Other Error Code)";
		printf("Error Code %d: \"%s\"", err_code, err_msg);
	}
	FindClose(hFind);
}
void c_cat(Command* cmd) {
	if (cmd->n_args < 2) {
		printf("Not enough arguments"); 
	}
}
void c_cd(Command* cmd) {
	printf("\nfunc Call: Execute \"cd\"");
}
void c_touch(Command* cmd) {
	printf("\nfunc Call: Execute \"touch\"");
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