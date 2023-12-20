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
	{"cat", 2, 3, c_cat},
};

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
	
}
void c_cd(Command* cmd) {
	printf("\nfunc Call: Execute \"cd\"");
}
void c_touch(Command* cmd) {
	printf("\nfunc Call: Execute \"touch\"");
}
