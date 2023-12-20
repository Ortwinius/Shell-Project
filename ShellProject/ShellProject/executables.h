#ifndef EXECUTABLES_H
#define EXECUTABLES_H

#include <stdio.h>
#include <ctype.h>
#include <wchar.h>
#include "structs.h"

extern const Exec ExecTable[];

unsigned int getFileSize(const char* searchpath, WCHAR* filename);
char* c_cwd(void);
void c_echo(Command* cmd);
void c_cat(Command* cmd);
void c_cd(Command* cmd);
void c_touch(Command* cmd);
void c_exit(void);
void c_help(void);
void c_ls(void);

#endif //EXECUTABLES_H
