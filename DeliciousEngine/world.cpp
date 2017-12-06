#include "world.h"

#include "resources.h"
#include "screen.h"
#include "input.h"

#include "dmath.h"

bool World::init(System_Ref sys) {
	system = sys;

	return true;
}

//@TEMP
bool World::load_test() {
	Texture* default_texture = system.resources->load_texture("res/tile.tga");
	Shader*  default_shader = system.resources->load_shader("res/default.glsl");
	if (default_texture == nullptr || default_shader == nullptr) {
		return false;
	}
	default_material = system.resources->make_material("default", default_texture, default_shader);
	Mesh* cube = system.resources->fetch_mesh("primitive.cube");

	entity_a.get_renderer()->set(cube, default_material);
	entity_b = entity_a;

	entity_a.get_transform()->set_position(-1.0f, 0.0f, 0.0f);
	entity_b.get_transform()->set_position(1.0f, 0.0f, 0.0f);

	system.screen->get_camera()->transform_matrix() = glm::translate(glm::mat4(1.0f), { 0.0f, 0.0f, 2.0f });

	return true;
}

void World::clean_exit() {

}

void World::update() {
	//@TEMP: Should just be iterating through entities and calling scripts, this is just test logic for now...
	do_camera();

	glm::vec3 axis = glm::normalize(glm::vec3(0.0f, 2.0f, 0.0f));
	entity_a.get_transform()->rotate(0.1f, axis);
	entity_b.get_transform()->rotate(1.0f, axis);

	glm::vec3 angles_a = entity_a.get_transform()->get_euler_angles();
	glm::vec3 angles_b = entity_b.get_transform()->get_euler_angles();
	
	static float t = 0;
	t += 0.05f;
	float off = -0.7f;
	entity_a.get_transform()->set_position(math::sine(1.0f, t) + off, math::cosine(0.6f, t), 0.0f);
	
	glUseProgram(default_material->shader->id);
	GLuint uniform_highlight = glGetUniformLocation(default_material->shader->id, "highlight");
	if (entity_a.colliding_with(&entity_b)) {
		glUniform1f(uniform_highlight, 0.2f);
	}
	else {
		glUniform1f(uniform_highlight, 0.0f);
	}
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

void World::do_camera() {
	//@TEMP: Until scripting

	Camera* cam = system.screen->get_camera();
	Input* input = system.input;

	glm::vec3 cam_direction = { 0.0f, 0.0f, 0.0f };
	glm::vec3 cam_axis = { 0.0f, 1.0f, 0.0f };
	float cam_angle = 0.0f;

	if (input->get_key(SDLK_w)) cam_direction.z = -1.0f;
	if (input->get_key(SDLK_a)) cam_direction.x = -1.0f;
	if (input->get_key(SDLK_d)) cam_direction.x = 1.0f;
	if (input->get_key(SDLK_s)) cam_direction.z = 1.0f;

	if (glm::length(cam_direction) > 0.0f) {
		cam_direction = glm::normalize(cam_direction);
	}

	cam->transform_matrix() = glm::translate(cam->transform_matrix(), cam_direction * 0.05f);
	cam->transform_matrix() = glm::rotate(cam->transform_matrix(), -glm::radians(cam_angle), cam_axis);
}

Entity* World::GetEntityByIndex(uint i) {
	return (i < entities.size()) ? &entities[i] : nullptr;
}

Entity* World::GetEntityByID(uint id) {
	for (auto& ent : entities) {
		if (ent.get_id() == id) return &ent;
	}
	return nullptr;
}

Entity* World::CreateEntity() {
	entities.push_back(Entity());
	return &entities.back();
}

Entity* World::AddEntity(Entity ent) {
	entities.push_back(ent);
	return &entities.back();
}