#ifndef DELICIOUS_STD_CVARS_H
#define DELICIOUS_STD_CVARS_H

#include <vector>
#include "console_types.h"

std::vector<console_var> standard_cvars = {
	{ "vid_init", 0, VAR_BOOL, CVAR_SYSTEM},
	{ "log_init", 0, VAR_BOOL, CVAR_SYSTEM},

	{ "vid_width", 1280, VAR_INT, CVAR_USER},
	{ "vid_height", 720, VAR_INT, CVAR_USER},
	{ "vid_fullscreen", 0, VAR_BOOL, CVAR_USER},
	{ "vid_borderless", 0, VAR_BOOL, CVAR_USER}
};

#endif