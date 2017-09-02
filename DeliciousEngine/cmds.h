#ifndef DELICIOUS_CMDS_H
#define DELICIOUS_CMDS_H

#include "console_types.h"
#include <vector>

extern std::vector<console_cmd> default_cmds;

//
// Default console command declarations
//
#define COMMAND_LIST\
	CMD(clear)\

#endif