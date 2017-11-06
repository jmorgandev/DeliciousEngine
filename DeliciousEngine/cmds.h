#ifndef DELICIOUS_CMDS_H
#define DELICIOUS_CMDS_H

#include "console_types.h"
#include <vector>
#include <array>

extern std::vector<console_cmd> default_cmds;

//
// Default console command declarations
// @TODO - Store these somewhere else, and stop using X macro
//
#define COMMAND_LIST\
	CMD(clear)\
	CMD(add)\
	CMD(quit)\
	CMD(toggleconsole)\
	CMD(resize)
#endif