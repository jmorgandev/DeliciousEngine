#include "resources.h"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <vec4.hpp>
#include <vec3.hpp>
#include <mat4x4.hpp>

#include "dgl.h"
#include "default_mesh.h"
#include "engine.h"
#include "console.h"

bool Resources::load() {
	return true;
}

bool Resources::free() {
	for (auto& item : mesh_catalog) {
		glDeleteBuffers(ATTRIBUTE_COUNT, item.second.vbo);
		glDeleteVertexArrays(1, &item.second.vao);
	}
	mesh_catalog.clear();

	for (auto& item : shader_catalog) {
		glDeleteShader(item.second.id);
	}
	shader_catalog.clear();

	for (auto& item : texture_catalog) {
		glDeleteTextures(1, &item.second.id);
	}
	texture_catalog.clear();

	return true;
}

Texture* Resources::load_texture(std::string filename, std::string id) {
	auto& console = engine.get<Console>();
	//int x, y, channels, desired channels
	int w, h, channels;
	byte* pixel_data = stbi_load(filename.c_str(), &w, &h, &channels, NULL);
	if (pixel_data == nullptr) {
		console.printf("Error loading \"%s\": %s", filename.c_str(), stbi_failure_reason());
		return nullptr;
	}
	stbi__vertical_flip(pixel_data, w, h, channels);

	GLuint texture_handle;
	glGenTextures(1, &texture_handle);
	glBindTexture(GL_TEXTURE_2D, texture_handle);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, w, h);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixel_data);

	stbi_image_free(pixel_data);

	texture_catalog[id] = { texture_handle, w, h, 4 };
	return &texture_catalog[id];
}

Texture* Resources::fetch_texture(std::string filename) {
	auto it = texture_catalog.find(filename);
	if (it == texture_catalog.end()) {
		return nullptr;
	}
	else {
		return &it->second;
	}
}

Shader* Resources::load_shader(std::string filepath) {
	Shader new_shader = {};

	std::string shader_src = dff::file_str(filepath);
	if (shader_src.empty()) {
		return nullptr;
	}
	std::string vertex_src = dff::get_glsl_region(shader_src, "vertex");
	std::string fragment_src = dff::get_glsl_region(shader_src, "fragment");
	if (vertex_src.empty() || fragment_src.empty()) {
		return nullptr;
	}
	std::string header_src = dff::get_glsl_header(shader_src);

	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	if (!vertex_shader || !fragment_shader) {
		return nullptr;
	}
	if (!dgl::compile(vertex_shader, header_src + vertex_src)) {
		GLchar buffer[1024];
		glGetShaderInfoLog(vertex_shader, 1024, NULL, buffer);
		std::cout << buffer << "\n";
		return nullptr;
	}
	if (!dgl::compile(fragment_shader, header_src + fragment_src)) {
		GLchar buffer[1024];
		glGetShaderInfoLog(fragment_shader, 1024, NULL, buffer);
		std::cout << buffer << "\n";
		return nullptr;
	}
	GLuint shader_program = glCreateProgram();
	if (!shader_program) {
		return nullptr;
	}
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);

	glLinkProgram(shader_program);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	new_shader.id = shader_program;
	shader_catalog[filepath] = new_shader;
	return &shader_catalog[filepath];
}

Shader* Resources::fetch_shader(std::string filename) {
	auto it = shader_catalog.find(filename);
	if (it == shader_catalog.end()) {
		return nullptr;
	}
	else {
		return &it->second;
	}
}

Mesh* Resources::fetch_mesh(std::string filename) {
	auto it = mesh_catalog.find(filename);
	if (it == mesh_catalog.end()) {
		return nullptr;
	}
	else {
		return &it->second;
	}
}

Mesh* Resources::make_mesh(std::string name, MeshData data) {
	Mesh new_mesh = {};

	// Create VBO
	glGenBuffers(1, &new_mesh.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, new_mesh.vbo);

	// Calculate VBO size and request immutable storage on GPU
	int element_count = data.vertices.size();

	std::vector<Vertex_VNCT> buffer_data(element_count);

	// Regularize Mesh to conform to VNCT format (Vertex,Normal,Color,Texcoord)
	if (data.normals.size() != element_count) {
		data.normals.clear();
		data.normals.resize(element_count, glm::vec3{ 0.0f });
	}
	if (data.texcoords.size() != element_count) {
		data.texcoords.clear();
		data.texcoords.resize(element_count, glm::vec2{ 0.0f });
	}
	std::vector<glm::vec4> colors(element_count, glm::vec4{ 1.0f }); // Temp colors

	// Transfer MeshData to CPU buffer (interleaved) (e.g. VNCTVNCT instead of VVNNCCTT)
	for (int i = 0; i < element_count; i++) {
		buffer_data[i].vertex = data.vertices[i];
		buffer_data[i].normal = data.normals[i];
		buffer_data[i].color = colors[i];
		buffer_data[i].texcoord = data.texcoords[i];
	}

	// Upload CPU buffer to GPU VBO
	glBufferStorage(GL_ARRAY_BUFFER, sizeof(Vertex_VNCT) * element_count, buffer_data.data(), NULL);

	new_mesh.vertex_count = element_count;
	new_mesh.stride = sizeof(Vertex_VNCT);
	mesh_catalog[name] = new_mesh;
	return &mesh_catalog[name];
}

/*
Mesh* Resources::make_mesh(std::string name, MeshData data) {
	//@Todo: VAOs should be for vertex attrib formatting, not PER mesh
	//@Todo: Store multiple data per VBO? VBO per Mesh?
	//@Todo: Auto fill mesh with dummy information to make compatible with VAO format
	Mesh new_mesh = {};

	//Create VAO
	glGenVertexArrays(1, &new_mesh.vao);
	glBindVertexArray(new_mesh.vao);

	//Create VBOs
	glGenBuffers(ATTRIBUTE_COUNT, new_mesh.vbo);

	// VERTICES
	glBindBuffer(GL_ARRAY_BUFFER, new_mesh.vbo[VERTICES]);
	glBufferData(GL_ARRAY_BUFFER, data.vertices.size() * sizeof(glm::vec3), &data.vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	// NORMALS
	glBindBuffer(GL_ARRAY_BUFFER, new_mesh.vbo[NORMALS]);
	glBufferData(GL_ARRAY_BUFFER, data.normals.size() * sizeof(glm::vec3), &data.normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);
	
	// TEXCOORDS
	glBindBuffer(GL_ARRAY_BUFFER, new_mesh.vbo[TEXCOORDS]);
	glBufferData(GL_ARRAY_BUFFER, data.texcoords.size() * sizeof(glm::vec2), &data.texcoords[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(2);

	new_mesh.vertex_count = (uint32)data.vertices.size();
	new_mesh.triangle_count = new_mesh.vertex_count / 3;
	
	mesh_catalog[name] = new_mesh;
	return &mesh_catalog[name];
}

Material* Resources::make_material(std::string name, Shader* shader) {
	material_catalog.emplace(name, shader);
	return &material_catalog[name];
}

Material* Resources::fetch_material(std::string filename) {
	auto it = material_catalog.find(filename);
	if (it == material_catalog.end()) {
		return nullptr;
	}
	else {
		return &it->second;
	}
}

bool Resources::load_default_resources() {
	make_mesh("primitive.quad", default_quad_mesh);
	make_mesh("primitive.cube", default_cube_mesh);
	return true;
}