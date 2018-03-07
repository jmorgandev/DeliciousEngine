#ifndef DELICIOUS_BUILD_INFO_H
#define DELICIOUS_BUILD_INFO_H

#include "dcm.h"

//Main version declaration
#define ENG_VER_MAJOR 0
#define ENG_VER_MINOR 0
#define ENG_VER_PATCH 8

#define ENG_VERSION_STR \
	TO_STR(ENG_VER_MAJOR) "." TO_STR(ENG_VER_MINOR) "." TO_STR(ENG_VER_PATCH)

#define DEFAULT_WIN_TITLE "Delicious Engine " ENG_VERSION_STR

#define ENGINE_GL_MAJOR 4
#define ENGINE_GL_MINOR 3

#define DEFAULT_GLSL_VERSION 430
#define DEFAULT_GLSL_STR "#version " TO_STR(DEFAULT_GLSL_VERSION) "\n"

#define USE_SCRIPTING false

/*========================== 0.0.8 CHANGELOG =========================

 * Added ImGui Library (For "immediate-mode" style rendering of gui)

 * Changed the console to use ImGui

 * Removed unnecessary gui renderers (BoxRenderer, FontRenderer)

 * Small systems refactor (Engine is no longer a class, but this can be reverted)

 * Added stb_* libraries

 * Added support for stb_image in resource system

 * Restructered project directories to better reflect what is happening

 * Added coding style document to reflect the style of the source code

  ====================================================================
*/

#endif