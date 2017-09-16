#ifndef DELICIOUS_CONSOLE_TYPES_H
#define DELICIOUS_CONSOLE_TYPES_H

#include "dtypes.h"
#include "system_ref.h"

#define CON_MAX_NAME	32

#define CVAR_SYSTEM		0x00					//Cannot be edited by the user, or written to config file
#define CVAR_MUTABLE	0x01					//Can be edited during runtime
#define CVAR_CONFIG		0x02					//Is written to the config file
#define CVAR_USER	(CVAR_MUTABLE | CVAR_CONFIG)//Can be edited by the user and is written to the config file

enum cvar_type {CVAR_BOOL, CVAR_FLOAT, CVAR_INT};	//The cvar type determines how the data value is restricted
union var_data {
	int   as_int;
	float as_float;
	bool  as_bool;
	var_data(int value) { as_int = value; }
	var_data(float value) { as_float = value; }
	var_data(bool value) { as_bool = value; }
};
struct console_var {
	char	    name[CON_MAX_NAME];
	cvar_type   type;
	var_data value;
	uint16	    flags;
};

typedef void(*cmd_callback)(System_Ref, char*, int);
struct console_cmd {
	char name[CON_MAX_NAME];
	cmd_callback callback;
	bool lua_call;
};

#endif