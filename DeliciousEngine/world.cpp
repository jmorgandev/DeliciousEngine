#include "world.h"

#include "resources.h"
#include "screen.h"
#include "input.h"

#include "dmath.h"

bool World::init(System_Ref sys) {
	system = sys;

	return true;
}

void World::load_test() {
	entity_a.get_renderer()->set_mesh(system.resources->fetch_mesh("primitive.cube"));
	entity_a.get_renderer()->set_shader(system.resources->load_shader("res/default.glsl"));
	entity_a.get_renderer()->set_texture(system.resources->load_texture("res/tile.tga"));
	entity_b = entity_a;

	entity_a.get_transform()->set_position(-1.0f, 0.0f, 0.0f);
	entity_b.get_transform()->set_position(1.0f, 0.0f, 0.0f);

	system.screen->get_camera()->transform_matrix() = glm::translate(glm::mat4(1.0f), { 0.0f, 0.0f, 2.0f });
}

void World::clean_exit() {

}

void World::update() {
	//@TEMP
	glm::vec3 axis = glm::normalize(glm::vec3(0.0f, 2.0f, 0.0f));
	glm::vec3 angles = axis * 0.01f;
	entity_a.get_transform()->rotate(0.01f, glm::normalize(angles));
	entity_b.get_transform()->rotate(angles);

	glm::vec3 angles_a = entity_a.get_transform()->get_euler_angles();
	glm::vec3 angles_b = entity_b.get_transform()->get_euler_angles();

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
	if (glm::length(cam_direction) > 0.0f) {
		cam_direction = glm::normalize(cam_direction);
	}

	//cam->look_at(entity_a.get_transform()->get_position());
	
	cam->transform_matrix() = glm::translate(cam->transform_matrix(), cam_direction * 0.005f);
	cam->transform_matrix() = glm::rotate(cam->transform_matrix(), -glm::radians(cam_angle), cam_axis);
}

void World::draw() {
	//@TEMP
	Camera* cam = system.screen->get_camera();
	glm::mat4 transform_a = entity_a.get_transform()->get_matrix();
	glm::mat4 transform_b = entity_b.get_transform()->get_matrix();

	glm::mat4 projection = cam->projection_matrix();
	glm::mat4 view = cam->view_matrix();

	entity_a.get_renderer()->draw(transform_a, view, projection);
	entity_b.get_renderer()->draw(transform_b, view, projection);
}