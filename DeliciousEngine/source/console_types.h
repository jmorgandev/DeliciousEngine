#ifndef DELICIOUS_CONSOLE_TYPES_H
#define DELICIOUS_CONSOLE_TYPES_H

#include <functional>

#include "dtypes.h"
#include "system_var.h"
#include <vector>
#include "build_settings.h"

constexpr uint CVAR_SYSTEM  = 0;       //Cannot be edited by the user, or written to config file
constexpr uint CVAR_MUTABLE = 1;       //Can be edited during runtime
constexpr uint CVAR_CONFIG  = 1 << 1;  //Is written to the config file
constexpr uint CVAR_USER = (CVAR_MUTABLE | CVAR_CONFIG); //Can be edited during runtime and is written to the config file

enum CvarType {CVAR_BOOL, CVAR_FLOAT, CVAR_INT, CVAR_STRING};
struct ConsoleVar {
	char name[CON_MAX_NAME];
	SystemVar* data;
	CvarType type;
	uint16 flags;
};

enum CmdType {CMD_CPP, CMD_LUA, CMD_CHAI};
typedef std::function<void(std::vector<cstring>)> CmdFunc;
struct ConsoleCmd {
	char name[CON_MAX_NAME];
	CmdFunc callback;
	CmdType type;
};

#endif