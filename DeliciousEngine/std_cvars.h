#ifndef DELICIOUS_STD_CVARS_H
#define DELICIOUS_STD_CVARS_H

#include "console_types.h"

console_var standard_cvars[] = {
	{ "log_init", 0, VAR_BOOL, CVAR_SYSTEM},
	{ "vid_init", 0, VAR_BOOL, CVAR_SYSTEM},

	{ "vid_width", 800, VAR_INT, CVAR_USER},
	{ "vid_height", 600, VAR_INT, CVAR_USER},
	{ "vid_fullscreen", 0, VAR_BOOL, CVAR_USER},
	{ "vid_borderless", 0, VAR_BOOL, CVAR_USER},

	{ "res_log", 0, VAR_BOOL, CVAR_USER}
};

#endif