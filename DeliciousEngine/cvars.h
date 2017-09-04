#ifndef DELICIOUS_CVARS_H
#define DELICIOUS_CVARS_H

#include "console_types.h"

console_var default_cvars[] = {
	{ "log_init", CVAR_BOOL, false, CVAR_SYSTEM},
	{ "vid_init", CVAR_BOOL, false, CVAR_SYSTEM},

	{ "vid_width", CVAR_INT, 800, CVAR_CONFIG},
	{ "vid_height", CVAR_INT, 600, CVAR_CONFIG },
	{ "vid_fullscreen", CVAR_BOOL, false, CVAR_CONFIG },
	{ "vid_borderless", CVAR_BOOL, false, CVAR_CONFIG },

	{"timescale", CVAR_FLOAT, 1.0f, CVAR_MUTABLE}
};

#endif