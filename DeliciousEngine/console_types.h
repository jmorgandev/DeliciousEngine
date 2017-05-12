#ifndef DELICIOUS_CONSOLE_TYPES_H
#define DELICIOUS_CONSOLE_TYPES_H

#include "dtypes.h"

#define CON_MAX_NAME	32

enum cvar_type {VAR_BOOL, VAR_FLOAT, VAR_INT};
struct console_var {
	char	  name[CON_MAX_NAME];
	float	  data;
	cvar_type type;
};

#endif