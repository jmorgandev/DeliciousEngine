#ifndef DELICIOUS_CONSOLE_TYPES_H
#define DELICIOUS_CONSOLE_TYPES_H

#include "dtypes.h"

#define CON_MAX_NAME	32

#define CVAR_SYSTEM	0x00						//If it cannot be edited by the user, or written to config file
#define CVAR_EDIT	0x01						//Can be edited during runtime
#define CVAR_OUT	0x02						//Is written to the config file
#define CVAR_USER	(CVAR_EDIT | CVAR_OUT)	//Can be edited by the user and is output to the config file

enum cvar_type {VAR_BOOL, VAR_FLOAT, VAR_INT};	//The cvar type determines how the data value is restricted
struct console_var {
	char	  name[CON_MAX_NAME];
	float	  data;
	cvar_type type;
	uint16	  flags;
};

#endif