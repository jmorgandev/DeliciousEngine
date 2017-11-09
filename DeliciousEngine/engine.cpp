#include "engine.h"

#include <SDL/SDL_events.h>
#include <fstream>
#include "mesh_renderer.h"
#include "dmath.h"
#include "system_ref.h"

#include <glm/gtc/matrix_transform.hpp>

Engine::Engine() {
	running = false;
	strict_mode  = false;
}

bool Engine::init(char** argv, int argc) {
	System_Ref systems;
	systems.console	  = &console;
	systems.screen    = &screen;
	systems.resources = &resources;

	//@TODO: Parse command line arguments.
	//	--strict   = Start the engine in strict mode
	//  --gl <num> = Override the OpenGL context version
	//  --fast     = Forces all graphics settings to their lowest/fastest
	//  --pretty   = Forces all graphics settings to their highest
	//  --dev	   = Load a blank world and open the console on startup

	if (console.init(systems) == false) return false;
	if (screen.init(systems) == false) return false;
	if (resources.init(systems) == false) return false;
	if (input.init(systems) == false) return false;

	//@TODO: Load config file with console
	if (screen.create_window() == false) return false;
	if (resources.load_default_resources() == false) return false;

	console.register_variable("eng_running", &running,     CVAR_BOOL, CVAR_SYSTEM);
	console.register_variable("eng_strict",  &strict_mode, CVAR_BOOL, CVAR_USER);

	return true;
}

void Engine::run() {
	running = true;

	//@TEMP
	Camera test_camera;
	test_camera.init(console.read_variable("vid_fov"), console.read_variable("vid_aspect"));
	test_camera.transform_matrix() = glm::translate(glm::mat4(1.0f), { 0.0f, 0.0f, 2.0f });

	glm::mat4 transform_matrix = glm::translate(glm::mat4(1.0f), { 0.0f, 0.0f, 0.0f });

	glm::vec3 rotation_axis = glm::normalize(glm::vec3(1.0f, 1.7f, 1.42f));

	MeshRenderer renderer;

	Shader* shader = resources.load_shader("res/default.glsl");
	Mesh*   mesh = resources.fetch_mesh("primitive.cube");
	Texture* texture = resources.load_texture("res/tile.tga");

	renderer.set(mesh, shader);
	renderer.set_texture(texture);

	while (running.as_bool == true) {
		test_camera.update();
		input.process_events();
		//@TEMP
		transform_matrix = glm::rotate(transform_matrix, 0.0005f, rotation_axis);
		renderer.draw(transform_matrix, test_camera.view_matrix(), test_camera.projection_matrix());
		console.render();
		screen.update();
	}
}

void Engine::shutdown() {
	input.stop();
	resources.close();
	screen.destroy();
	console.terminate();
}