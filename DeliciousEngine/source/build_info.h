#ifndef DELICIOUS_BUILD_INFO_H
#define DELICIOUS_BUILD_INFO_H

#include "dcm.h"
#include "build_settings.h"

#define ENG_VERSION_STR \
	TO_STR(ENG_VER_MAJOR) "." TO_STR(ENG_VER_MINOR) "." TO_STR(ENG_VER_PATCH)

#define DEFAULT_WIN_TITLE "Delicious Engine " ENG_VERSION_STR

#define DEFAULT_GLSL_STR "#version " TO_STR(DEFAULT_GLSL_VERSION) "\n"

/*==================== CHANGELOG (0.1.2) =============================

 * Removed scripting, focus on cpp interface

 * Changed console command registering to support lua functions

 * Scripting system can load a start script, and calls OnTick every update

 * sol2 lua binds for glm types

 * sol2 lua binds for entities and components

 * sol2 lua binds for resource loading


 *====================================================================
*/

//@Bug: Material uniform block does not update dynamically?

#endif