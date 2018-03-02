#ifndef DELICIOUS_BUILD_INFO_H
#define DELICIOUS_BUILD_INFO_H

#include "dcm.h"

// Main version definition
#define ENGINE_VERSION 008

#define ENGINE_VERSION_MAJOR ENGINE_VERSION / 100
#define ENGINE_VERSION_MINOR ENGINE_VERSION / 10 % 10
#define ENGINE_VERSION_REVISION ENGINE_VERSION % 10
#define ENGINE_VERSION_STR \
	TO_STR(ENGINE_VERSION_MAJOR) "."\
	TO_STR(ENGINE_VERSION_MINOR) "."\
	TO_STR(ENGINE_VERSION_PATCH)

#define ENGINE_GL_MAJOR 4
#define ENGINE_GL_MINOR 3

#define ENGINE_GLSL_VERSION 430
#define ENGINE_GLSL_VERSION_STRING "#version 430 core"

#define USE_SCRIPTING false

/*========================== 0.0.8 CHANGELOG =========================

 * Added ImGui Library (For "immediate-mode style" rendering of gui)

 * Changed the console to use ImGui

 * Removed unnecessary gui renderers (BoxRenderer, FontRenderer)

 * Small systems refactor (Engine is no longer a class, but this could be reverted)

 * Added stb_* libraries

 * Added support for stb_image in resource system

 * Restructered project directories to better reflect what is happening

  ====================================================================
*/

#endif