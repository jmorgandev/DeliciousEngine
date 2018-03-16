#ifndef DELICIOUS_CONSOLE_TYPES_H
#define DELICIOUS_CONSOLE_TYPES_H

#include "dtypes.h"
#include "system_var.h"
#include <vector>

constexpr uint CON_MAX_NAME = 32;

constexpr uint CVAR_SYSTEM  = 0;       //Cannot be edited by the user, or written to config file
constexpr uint CVAR_MUTABLE = 1;       //Can be edited during runtime
constexpr uint CVAR_CONFIG  = 1 << 1;  //Is written to the config file
constexpr uint CVAR_USER = (CVAR_MUTABLE | CVAR_CONFIG); //Can be edited during runtime and is written to the config file

enum cvar_type {CVAR_BOOL, CVAR_FLOAT, CVAR_INT}; //The cvar type determines how the data value is restricted
struct console_var {
	char		name[CON_MAX_NAME];
	system_var* value;
	cvar_type	type;
	uint16		flags;
};

typedef void(*cmd_callback)(std::vector<cstring>);
struct console_cmd {
	char name[CON_MAX_NAME];
	cmd_callback callback;
};

#endif