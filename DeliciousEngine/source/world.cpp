#include "world.h"

#include <algorithm>

#include "resources.h"
#include "screen.h"
#include "input.h"
#include "dmath.h"
#include "material.h"

bool World::init() {
	return true;
}

//@Temp
bool World::load_test() {
	//Texture* default_texture = system.resources->load_texture("res/tile.tga");
	Texture* default_texture = resources.load_texture("res/tile.png");

	Texture* other_texture = resources.load_texture("res/consolas_32.tga", "tile");

	Shader*  default_shader = resources.load_shader("res/default.glsl");
	if (default_texture == nullptr || default_shader == nullptr) {
		return false;
	}
	default_material = resources.make_material("default", default_shader);
	default_material->set_vec4("diffuse_tint", 0.5f, 1.0f, 1.0f, 1.0f);
	default_material->set_texture("diffuse", default_texture);
	Mesh* cube = resources.fetch_mesh("primitive.cube");

	other_material = resources.make_material("other", default_shader);
	other_material->set_texture("diffuse", other_texture);

	//Entity first, second;
	Entity* first = create_entity("first");
	first->get_renderer()
		.set_mesh(cube)
		.set_material(default_material);
	first->get_transform().set_position(-1.0f, 0.0f, 0.0f);

	Entity* second = clone_entity(first, { 1.0f, 0.0f, 0.0f });
	second->get_renderer()
		.set_mesh(cube)
		.set_material(other_material);
	second->set_name("second");

	Entity* third = clone_entity(second, { 0.0f, 0.0f, 0.0f });
	third->set_name("third");

	screen.get_camera()->transform_matrix() = glm::translate(glm::mat4(1.0f), { 0.0f, 0.0f, 2.0f });
	return true;
}

void World::clean_exit() {

}

void World::update() {
	//@Temp: Should just be iterating through entities and calling scripts, this is just test logic for now...
	do_camera();

	Entity* first = get_entity(0);
	Entity* second = get_entity(1);

	glm::vec3 axis = glm::normalize(glm::vec3(1.0f, 2.0f, 1.2f));
	first->get_transform().rotate(0.5f, axis);
	second->get_transform().rotate(1.0f, axis);
	
	static float t = 0;
	t += 0.05f;
	float off = -0.7f;
	first->get_transform().set_position(0.0f, math::cosine(0.6f, t), 0.0f);
	second->get_transform().set_position(math::sine(0.2f, t) + 1, math::cosine(0.2f, t), 0.0f);
	//glUseProgram(default_material->shader->id);
	//GLuint uniform_highlight = glGetUniformLocation(default_material->shader->id, "highlight");
	//if (collision(first, second)) {
	//	glUniform1f(uniform_highlight, 0.2f);
	//}
	//else {
	//	glUniform1f(uniform_highlight, 0.0f);
	//}
}

void World::draw() {
	//@Temp
	Entity* first = get_entity(0);
	Entity* second = get_entity(1);
	Entity* third = get_entity(2);

	Camera* cam = screen.get_camera();
	glm::mat4 transform_a = first->get_transform().get_matrix();
	glm::mat4 transform_b = second->get_transform().get_matrix();
	glm::mat4 transform_c = third->get_transform().get_matrix();

	glm::mat4 projection = cam->projection_matrix();
	glm::mat4 view = cam->view_matrix();

	default_material->set_matrix("projection", projection);
	default_material->set_matrix("view", view);

	//other_material->set_matrix("projection", projection);
	//other_material->set_matrix("view", view);
	//
	//other_material->set_matrix("transform", transform_b);
	//second->get_renderer()->draw();
	//other_material->set_matrix("transform", transform_c);
	//third->get_renderer()->draw();

	default_material->set_matrix("transform", transform_a);
	first->get_renderer().draw();
}

void World::do_camera() {
	//@Temp: Until scripting

	Camera* cam = screen.get_camera();

	glm::vec3 cam_direction = { 0.0f, 0.0f, 0.0f };
	glm::vec3 cam_axis = { 0.0f, 1.0f, 0.0f };
	float cam_angle = 0.0f;

	if (input.get_key(SDLK_w)) cam_direction.z = -1.0f;
	if (input.get_key(SDLK_a)) cam_direction.x = -1.0f;
	if (input.get_key(SDLK_d)) cam_direction.x = 1.0f;
	if (input.get_key(SDLK_s)) cam_direction.z = 1.0f;

	if (glm::length(cam_direction) > 0.0f) {
		cam_direction = glm::normalize(cam_direction);
	}

	cam->transform_matrix() = glm::translate(cam->transform_matrix(), cam_direction * 0.05f);
	cam->transform_matrix() = glm::rotate(cam->transform_matrix(), -glm::radians(cam_angle), cam_axis);
}

Entity* World::get_entity(uint index) {
	if (index < entities.size()) {
		auto it = entities.begin();
		std::advance(it, index);
		return &(*it);
	}
	return nullptr;
}

Entity* World::create_entity(std::string name) {
	entities.emplace_back(name);
	return &entities.back();
}

Entity* World::copy_entity(Entity* ent) {
	entities.emplace_back(*ent);
	return &entities.back();
}

Entity* World::clone_entity(Entity* ent, glm::vec3 pos) {
	entities.emplace_back(*ent);
	entities.back().get_transform().set_position(pos);
	return &entities.back();
}
Entity* World::clone_entity(Entity* ent, glm::vec3 pos, glm::quat rot) {
	entities.emplace_back(*ent);
	Transform& t = entities.back().get_transform();
	t.set_position(pos);
	t.set_rotation(rot);
	return &entities.back();
}
Entity* World::clone_entity(Entity* ent, glm::vec3 pos, glm::vec3 scale) {
	entities.emplace_back(*ent);
	Transform& t = entities.back().get_transform();
	t.set_position(pos);
	t.set_scale(scale);
	return &entities.back();
}
Entity* World::clone_entity(Entity* ent, glm::vec3 pos, glm::quat rot, glm::vec3 scale) {
	entities.emplace_back(*ent);
	Transform& t = entities.back().get_transform();
	t.set_position(pos);
	t.set_rotation(rot);
	t.set_scale(scale);
	return &entities.back();
}
Entity* World::clone_entity(Entity* ent, Transform tfm) {
	entities.emplace_back(*ent);
	entities.back().get_transform() = tfm;
	return &entities.back();
}

void World::destroy_entity(Entity* ent) {
	for (auto it = entities.begin(); it != entities.end();) {
		if (ent == &(*it)) {
			entities.erase(it);
			return;
		}
		else ++it;
	}
}