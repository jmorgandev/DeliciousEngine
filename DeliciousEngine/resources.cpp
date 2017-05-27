#include "resources.h"

#include <SDL/SDL.h>
#include "engine.h"
#include "console.h"
#include "dff.h"
#include "dcf.h"

bool Resources::init(Engine* engine_in) {
	console_ref = engine_in->get_console();
	return true;
}

void Resources::load_texture(cstring filepath) {

	if (dff::path_extension(filepath) != "png") {
		//Console error log
		return;
	}



}