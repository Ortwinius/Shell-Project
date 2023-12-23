#include "libs.h"
#include "cmd.h"
#include "structs.h"
#include "executables.h"
#include "constants.h"

int in_range(int ref, int min, int max) {
	return (ref >= min && ref <= max) ? 1 : 0;
}

int executeCmd(Command* cmd) {
	//->Std Executables gets recognized and the corresponding function gets called
	char* cmd_title = _strdup(cmd->args[0]);
	for (int i = 0; i < CMD_LIST; i++) {
		if (strcmp(ExecTable[i].cmd, cmd_title) == 0
			&& in_range(cmd->n_args, ExecTable[i].min_args, ExecTable[i].max_args))
		{
			ExecTable[i].handler(cmd);
			return 1;
		}
	}
	//TODO: Alternative cmds (like ? ) will be looked for depending on special signs


	printf("\nError: Command not found or invalid number of arguments. Type \"help\" for assistance.");
	return 1;
}