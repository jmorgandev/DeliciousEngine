#include "engine.h"

bool Engine::init(char** argv, int argc) {
	if (!console.init(this)) {
		return false;
	}
	if (!screen.init(this)) {
		return false;
	}
	
	return true;
}

void Engine::run() {
	//
	// @Todo main loop
	//
}

void Engine::clean() {
	//
	// @Todo cleanup operations
	//
}