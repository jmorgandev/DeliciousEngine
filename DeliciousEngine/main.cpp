#include "engine.h"
#include "dtypes.h"
#include <iostream>


int main(char** argv, int argc) {	
	Engine delicious_engine;

	if (delicious_engine.init(argv, argc)) {
		delicious_engine.run();
	}
	delicious_engine.shutdown();

	return 0;
}

/* ==== NOTES ====

FMOD vs SDL_Mixer vs Wwise

Multithreading:
    -locks
	-mutexes
	-condition variables

Newton dynamics

Advanced audio effects:
	-materials
	-pathfinding
	-special effects

Rendering:
	-deferred rendering
	-per-pixel lighting
	-shadow mapping
	-reflections
	-framebuffer effects
	-lightmapping

Angelscript vs Lua vs Python

Reloading C++ .dlls?

Hotloading resources:
	-sounds
	-meshes
	-textures
	-shaders

Custom extension workflows:
	-"FileLoaders"?
	-script extensions

Framerate/Timing:
	-FPS cap?
	-Vsync?
	-Determinism?
	-Different "timelines"
	-Delta time?
	-Physics time?
	-Fixed time updates?

Skinned animation? FBX? other animation formats?

User-accessible GUI?

*/