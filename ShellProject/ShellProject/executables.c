#include "libs.h"
#include "executables.h"
#include "constants.h"
#include "structs.h"

const Exec ExecTable[] = {
	{"exit", 1, 1, c_exit},
	{"help", 1, 3, c_help},
	{"cwd", 1, 1, c_cwd},
	{"ls", 1, 4, c_ls},
	{"cd", 2, 2, c_cd},
	{"echo", 2, MAX_LEN, c_echo},
	{"touch", 2, 3, c_touch},
	{"rm", 2, 2, c_rm},
	{"cat", 2, 3, c_cat},
	{"exec", 2, 2, c_exec},
	{"vaugh", 2, 3, c_vaugh},
};

/*
TODO COMMANDS:
tail
chmod
size
exec
pipeline? grep?
*/
//TODO: swap out errors into error.c

size_t getFileSize(wchar_t* searchpath) {
	/*A function to get the size of a file depending on the searchpath (in wchar_t format). 
	Not necessary because of WIN32_FIND_DATAW.fileSizeLow*/

	size_t file_size = 0;
	struct stat st;
	char convpath[MAX_LEN];

	//convert to char*
	wcstombs(convpath, searchpath, MAX_LEN);

	//If file size can be evaluated return it, else err
	if (stat(convpath, &st) != 0) {
		printf("\n[Error %d] ", GetLastError());
	}
	return st.st_size;
}
void c_exit(void) {
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
void c_exec(Command* cmd) {
	if (cmd->args[1] == NULL) {
		fprintf(stderr, "Invalid file name. Check the file path/name.");
		return;
	}
	char* fullPath = (char*)malloc(sizeof(char) * MAX_LEN);
	sprintf(fullPath, "%s", cmd->args[1]);

	int running = system(fullPath);

	if (running == ERROR_FILE_NOT_FOUND) {
		fprintf(stderr, "Could not find executable. Please check your file name / directory.");
	}
	free(fullPath);
}
void c_ls(void) {
	//wchar_t needed for WIN32_FIND_DATAW due to unicode character decoding
	wchar_t cwd[MAX_LEN];
	wchar_t file_n_buffer[MAX_LEN];
	wchar_t searchpath[MAX_LEN];
	HANDLE hFind;
	WIN32_FIND_DATAW f;

	_wgetcwd(cwd, sizeof(cwd) / sizeof(wchar_t));
	if (cwd == NULL) {
		fprintf(stderr, "Fatal error finding current path.");
		exit(EXIT_FAILURE);
	}
	swprintf(searchpath, MAX_LEN, L"%ls\\*", cwd);
	hFind = FindFirstFileW(searchpath, &f);

	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			
			swprintf(file_n_buffer, MAX_LEN, L"%ls\\%ls", cwd, f.cFileName);
			printf("\n%-10zu %ls", f.nFileSizeLow, f.cFileName);
			//wprintf(L"%ls", f.cFileName);
		} while (FindNextFileW(hFind, &f) != 0);
	}
	else {
		DWORD err_code = GetLastError();
		char* err_msg = (err_code == ERROR_FILE_NOT_FOUND) ? "No files found." : "Can't find given path";
		printf(L"Error Code %d: \"%s\"", err_code, err_msg);
	}
	FindClose(hFind);
}
void c_cat(Command* cmd) {
	char* fullPath = (char*)malloc(sizeof(char) * MAX_LEN);
	sprintf(fullPath, "%s", cmd->args[1]);
	FILE* fp;
	fp = fopen(fullPath, "r");
	if (fp == NULL) {
		fprintf(stderr, "Error opening the file \"%s\". Please check the file name.", fullPath);
		return;
	}
	//TODO: cat test.txt [arg]? Maybe create an arg to only read so many lines of text?
	printf("************************\n\t%s\n************************\n", fullPath);
	char c;
	while ((c = getc(fp)) != EOF) {
		printf("%c", c);
	}
	fclose(fp);
}
void c_vaugh(Command* cmd) {
	printf("Coming soon.");
	/*
	Vaugh: Vim-like basic text editor
	Functionality:
	*)Opens separate "window" (cls console)
	*)Lets the user type in input
	*)Special Chars get recognized (Such as Enter or Ctrl) to control where and how the input gets stored
	*)After finishing writing the user can save the file in the directory 
	*/
}
void c_cd(Command* cmd) {
	char* fullPath = (char*)malloc(sizeof(char) * MAX_LEN);
	sprintf(fullPath, "%s", cmd->args[1]);
	//TODO: Check if path is file, if yes -> error?
	int succ = _chdir(fullPath);
	if (succ != 0) {
		fprintf(stderr, "Could not change directory (Error code: %d)", succ);
	}
	free(fullPath);
}
void c_touch(Command* cmd) {
	//parsing of arg into dir and file not necessary
	//"Better" way: save full path using sprintf

	char* fullPath = (char*)malloc(sizeof(char) * MAX_LEN);
	sprintf(fullPath, "%s", cmd->args[1]);

	FILE* fp;
	fp = fopen(fullPath, "w");
	if (fp == NULL) {
		fprintf(stderr, "Error creating the file. Check if the directory exists or if you used invalid characters.");
		return;
	}
	printf("File successfully created");
	fclose(fp);
	free(fullPath);
}
void c_rm(Command* cmd) {
	//rm [path] -> Removing file depending on path
	char* fullPath = (char*)malloc(sizeof(char) * 50);

	//Save full path depending on arg 2
	sprintf(fullPath, "%s", cmd->args[1]);

	if (remove(fullPath) != 0) {
		fprintf(stderr, "Error removing file \"%s\". Check if the directory and the file exists.", cmd->args[1]);
		return;
	}
	printf("File successfully deleted.");
	free(fullPath);
}