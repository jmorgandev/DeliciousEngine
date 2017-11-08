#include "engine.h"

#include <SDL/SDL_events.h>
#include <fstream>
#include "mesh_renderer.h"
#include "dmath.h"
#include "system_ref.h"

#include <glm/gtc/matrix_transform.hpp>

Engine::Engine() {
	eng_running = false;
	eng_strict  = false;
}

bool Engine::init(char** argv, int argc) {
	System_Ref systems;
	systems.console	  = &console;
	systems.screen    = &screen;
	systems.resources = &resources;

	if (console.init(systems) == false) return false;
	if (screen.init(systems) == false) return false;
	if (resources.init(systems) == false) return false;
	if (input.init(systems) == false) return false;

	//@TODO - Parse command line arguments
	//	--strict   = Start the engine in strict mode
	//  --gl <num> = Override the OpenGL context version
	//  --fast     = Forces all graphics settings to their lowest/fastest
	//  --pretty   = Forces all graphics settings to their highest
	//  --dev	   = Load a blank world and open the console on startup

	//@TODO - Load config file with console
	if (screen.create_window() == false) return false;
	if (resources.load_default_resources() == false) return false;

	console.register_variable("eng_running", &eng_running, CVAR_BOOL, CVAR_SYSTEM);
	console.register_variable("eng_strict",  &eng_strict,  CVAR_BOOL, CVAR_USER);

	return true;
}

void Engine::run() {
	eng_running = true;

	//@TEMP
	glm::mat4 transform_matrix = glm::translate(glm::mat4(1.0f), { 0.0f, 0.0f, -5.0f });
	glm::mat4 projection_matrix = glm::perspective(75.0f, 640.0f / 480.0f, 1.0f, 1000.0f);
	glm::mat4 view_matrix = glm::inverse(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 5.0f)));

	glm::vec3 rotation_axis = glm::normalize(glm::vec3(1.0f, 1.7f, 1.42f));

	MeshRenderer renderer;

	Shader* shader = resources.load_shader("res/default.glsl");
	Mesh*   mesh = resources.fetch_mesh("primitive.cube");

	renderer.set(mesh, shader);

	while (eng_running.as_bool == true) {
		input.process_events();
		//@TEMP
		transform_matrix = glm::rotate(transform_matrix, 0.0001f, rotation_axis);
		renderer.draw(transform_matrix, view_matrix, projection_matrix);
		console.render();
		screen.update();
	}
}

void Engine::shutdown() {
	input.stop();
	resources.stop();
	screen.stop();
	console.stop();
}