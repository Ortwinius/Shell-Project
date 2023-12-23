#ifndef CMD_H
#define CMD_H

#include <stdio.h>
#include "structs.h"

int in_range(int ref, int min, int max);
int executeCmd(Command* cmd);

#endif //CMD_H