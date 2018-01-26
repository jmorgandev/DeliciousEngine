#ifndef DELICIOUS_BUILD_INFO_H
#define DELICIOUS_BUILD_INFO_H

#define ENGINE_VERSION	007
#define ENGINE_VERSION_MAJOR 0
#define ENGINE_VERSION_MINOR 0
#define ENGINE_VERSION_REVISION 7
#define ENGINE_VERSION_STRING "0.0.7"

#define ENGINE_GL_MAJOR 4
#define ENGINE_GL_MINOR 3

#define ENGINE_GLSL_VERSION 430
#define ENGINE_GLSL_VERSION_STRING "#version 430 core"

#define EXPOSE_GLOBAL_SYSTEM true
#define USE_SCRIPTING false

/*========================== 0.0.7 CHANGELOG =========================

 * Added time subsystem (Placeholder for now)

 * 60hz frame & sim locking (Decouple render rate and sim rate later)

 * Basic World-Entity management (Adding, Fetching, and Removing entities)

 * Add Material class
	-Uses UBOs to have "instances" of uniform sets amongst entities
	-Can just bind the material UBO rather than updating all the uniforms
	 at once using slow glUniform* calls

 * Added "header" scope to GLSL files. Anything typed above the first
   shader scope declaration will be copied into all shader scopes during
   compilation

 * Added ground work for multi-texturing (Sampler and bind tracking)

  ====================================================================
*/

#endif