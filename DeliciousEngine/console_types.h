#ifndef DELICIOUS_CONSOLE_TYPES_H
#define DELICIOUS_CONSOLE_TYPES_H

#include "dtypes.h"

#define CON_MAX_NAME	32

#define CVAR_SYSTEM	0x01						//variable cannot be edited by the user, or written to config file
#define CVAR_EDIT	0x02						//Can be edited during runtime
#define CVAR_WRITE	0x04						//Is written to the config file
#define CVAR_USER	(CVAR_EDIT | CVAR_WRITE)	//Can be edited by the user and is written to the config file

enum cvar_type {CVAR_BOOL, CVAR_FLOAT, CVAR_INT};	//The cvar type determines how the data value is restricted
struct console_var {
	char	  name[CON_MAX_NAME];
	cvar_type type;
	float	  value;
	uint16	  flags;
};

#endif