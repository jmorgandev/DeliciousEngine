#ifndef DELICIOUS_BUILD_INFO_H
#define DELICIOUS_BUILD_INFO_H

#define ENGINE_VERSION	005
#define ENGINE_VERSION_MAJOR 0
#define ENGINE_VERSION_MINOR 0
#define ENGINE_VERSION_REVISION 5
#define ENGINE_VERSION_STRING "0.0.5"

#define ENGINE_GL_MAJOR 4
#define ENGINE_GL_MINOR 3

#define ENGINE_GLSL_VERSION 430
#define ENGINE_GLSL_VERSION_STRING "#version 430 core"

#define EXPOSE_GLOBAL_SYSTEM true
#define USE_SCRIPTING false

/* ========================= 0.0.6 CHANGELOG =========================

 * Fixed console bug where line wrap point is incorrectly calculated.

 * Added World subsystem and Entity class, created a render test scene.

 * Added Material class (Placeholder for now).

 * Simple bounding sphere collision testing.

 * System variables can now be inspected and assigned to from within
   the console.
   e.g: "vid_fov 75"
   Some variables cannot be assigned to and have been deliberately
   marked for internal use only, such as "vid_aspect" and "vid_init".

*/

#endif