#ifndef DELICIOUS_BUILD_INFO_H
#define DELICIOUS_BUILD_INFO_H

#include "dcm.h"

//Main version declaration
#define ENG_VER_MAJOR 0
#define ENG_VER_MINOR 1
#define ENG_VER_PATCH 0

#define ENG_VERSION_STR \
	TO_STR(ENG_VER_MAJOR) "." TO_STR(ENG_VER_MINOR) "." TO_STR(ENG_VER_PATCH)

#define DEFAULT_WIN_TITLE "Delicious Engine " ENG_VERSION_STR

#define ENGINE_GL_MAJOR 4
#define ENGINE_GL_MINOR 3

#define DEFAULT_GLSL_VERSION 430
#define DEFAULT_GLSL_STR "#version " TO_STR(DEFAULT_GLSL_VERSION) "\n"

#define USE_SCRIPTING false

/*============================ CHANGELOG =============================

 * Changed console command registering to support lua functions

 * Scripting system can load a start script, and calls OnTick every update

 * sol2 lua binds for glm types

 * sol2 lua binds for entities and components

 * sol2 lua binds for resource loading

 * Basic time keeping inside World system

 *====================================================================
*/

//@Bug: Console scrolling does not work properly, clips strings too early...

#endif