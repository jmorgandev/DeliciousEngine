#include "world.h"

#include "resources.h"
#include "screen.h"
#include "input.h"

#include <glm/gtc/matrix_transform.hpp>

bool World::init(System_Ref sys) {
	system = sys;

	return true;
}

void World::load_test() {
	test_entity.get_transform()->set_position(0.0f, 0.0f, 0.0f);
	test_entity.get_renderer()->set_mesh(system.resources->fetch_mesh("primitive.cube"));
	test_entity.get_renderer()->set_shader(system.resources->load_shader("res/default.glsl"));
	test_entity.get_renderer()->set_texture(system.resources->load_texture("res/tile.tga"));

	system.screen->get_camera()->transform_matrix() = glm::translate(glm::mat4(1.0f), { 0.0f, 0.0f, 2.0f });
}

void World::clean_exit() {

}

void World::update() {
	//@TEMP
	glm::vec3 axis = glm::normalize(glm::vec3(0.5f, 2.0f, 1.0f));
	test_entity.get_transform()->rotate(0.005f, axis);

	Camera* cam = system.screen->get_camera();
	Input* input = system.input;

	glm::vec3 cam_direction = { 0.0f, 0.0f, 0.0f };
	glm::vec3 cam_axis = { 0.0f, 1.0f, 0.0f };
	float cam_angle = 0.0f;

	if (system.input->get_key(SDLK_w)) {
		cam_direction.z = -1.0f;
	}
	if (input->get_key(SDLK_a)) {
		cam_direction.x = -1.0f;
	}
	if (input->get_key(SDLK_d)) {
		cam_direction.x = 1.0f;
	}
	if (input->get_key(SDLK_s)) {
		cam_direction.z = 1.0f;
	}
	if (input->get_key(SDLK_RIGHT)) {
		cam_angle += 0.1f;
	}
	if (input->get_key(SDLK_LEFT)) {
		cam_angle -= 0.1f;
	}
	if (glm::length(cam_direction) > 0.0f) {
		cam_direction = glm::normalize(cam_direction);
	}
	
	cam->transform_matrix() = glm::translate(cam->transform_matrix(), cam_direction * 0.005f);
	cam->transform_matrix() = glm::rotate(cam->transform_matrix(), -glm::radians(cam_angle), cam_axis);
}

void World::draw() {
	//@TEMP
	Camera* cam = system.screen->get_camera();
	glm::mat4 transform = test_entity.get_transform()->get_matrix();
	glm::mat4 view = cam->view_matrix();
	glm::mat4 projection = cam->projection_matrix();

	test_entity.get_renderer()->draw(transform, view, projection);
}