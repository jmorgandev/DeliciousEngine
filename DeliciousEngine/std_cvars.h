#ifndef DELICIOUS_STD_CVARS_H
#define DELICIOUS_STD_CVARS_H

#include "console_types.h"

console_var standard_cvars[] = {
	{ "log_init", CVAR_BOOL, 0, CVAR_SYSTEM},
	{ "vid_init", CVAR_BOOL, 0, CVAR_SYSTEM},

	{ "vid_width", CVAR_INT, 800, CVAR_WRITE},
	{ "vid_height", CVAR_INT, 600, CVAR_WRITE},
	{ "vid_fullscreen", CVAR_INT, 0, CVAR_WRITE},
	{ "vid_borderless", CVAR_INT, 0, CVAR_WRITE}
};

#endif