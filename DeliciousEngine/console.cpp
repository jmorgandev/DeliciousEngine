#include "console.h"

#include "engine.h"

bool Console::init(Engine* engine_in) {
	engine = engine_in;

	return true;
}