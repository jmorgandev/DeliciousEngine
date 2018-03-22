#include "scripting.h"

#include <functional>

#include <vec3.hpp>
#include <gtc/quaternion.hpp>
#include <gtx/string_cast.hpp>
#include <mat4x4.hpp>

#include "console.h"
#include "texture.h"
#include "shader.h"
#include "transform.h"
#include "entity.h"
#include "resources.h"
#include "mesh_renderer.h"
#include "world.h"

//@Temp
static auto mult_overloads = sol::overload(
	[](const glm::vec3& v1, const glm::vec3& v2) -> glm::vec3 { return v1 * v2; },
	[](const glm::vec3& v1, const float& f) -> glm::vec3 { return v1 * f; },
	[](const float& f, glm::vec3& v1) -> glm::vec3 { return f * v1; }
);

static auto add_overloads = sol::overload(
	[](const glm::vec3& v1, const glm::vec3& v2) -> glm::vec3 {return v1 + v2; }
);

static auto set_overloads = sol::overload(
	sol::resolve<void(std::string, glm::vec4 value)>(&Material::set),
	sol::resolve<void(std::string, const Texture* tex)>(&Material::set)
);

static sol::protected_function_result error_handle(lua_State*, sol::protected_function_result pfr) {
	sol::error err = pfr;
	console.print(err.what());
	return pfr;
}

inline void my_panic(sol::optional<std::string> msg) {
	if (msg) {
		console.print(msg.value());
	}
}

bool Scripting::init() {
	lua.open_libraries(sol::lib::math);

	if (!bind_datatypes()) return false;
	if (!bind_components()) return false;
	if (!bind_systems()) return false;

	return true;
}

void Scripting::lua_print(sol::object obj) {
	if (obj.valid()) {
		sol::protected_function_result result = lua["tostring"](obj);
		if (result.valid()) {
			console.print(result.get<cstring>());
		}
		else {
			sol::error e = result;
			console.print(e.what());
		}
	}
	else {
		console.print("nil");
	}
}

bool Scripting::bind_datatypes() {
	//register glm
	lua.new_usertype<glm::vec3>(
		"vec3",
		sol::constructors<glm::vec3(), glm::vec3(float), glm::vec3(float, float, float)>(),
		"x", &glm::vec3::x,
		"y", &glm::vec3::y,
		"z", &glm::vec3::z,
		sol::meta_function::addition, [](const glm::vec3& v1, const glm::vec3& v2) -> glm::vec3 { return v1 + v2; }
	);

	lua.new_usertype<glm::vec4>(
		"vec4",
		sol::constructors<glm::vec4(), glm::vec4(float), glm::vec4(float, float, float, float)>(),
		"x", &glm::vec4::x,
		"y", &glm::vec4::y,
		"z", &glm::vec4::z,
		"w", &glm::vec4::w,
		sol::meta_function::addition, [](const glm::vec4& v1, const glm::vec4& v2) -> glm::vec4 { return v1 + v2; }
	);
	
	lua.new_usertype<glm::quat>(
		"quat",
		sol::constructors<glm::quat(), glm::quat(glm::vec3), glm::quat(float, float, float, float)>(),
		sol::meta_function::multiplication, [](const glm::quat& q1, const glm::quat& q2) -> glm::quat { return q1 * q2; }
	);

	lua.new_usertype<Texture>(
		"Texture",
		"new", sol::no_constructor,
		"width", &Texture::width,
		"height", &Texture::height
	);
	
	lua.new_usertype<Material>(
		"Material",
		"new", sol::no_constructor,
		sol::meta_function::new_index, set_overloads
	);

	lua.new_usertype<Shader>(
		"Shader",
		"new", sol::no_constructor
	);
	
	lua.new_usertype<Entity>(
		"Entity",
		"new", sol::no_constructor,
		"renderer", sol::readonly_property(&Entity::get_renderer),
		"transform", sol::readonly_property(&Entity::get_transform)
	);

	return true;
}

bool Scripting::bind_components() {

	lua.new_usertype<Transform>(
		"Transform",
		sol::constructors<Transform()>(),
		"position", sol::property(&Transform::get_position,
								  sol::resolve<void(const glm::vec3&)>(&Transform::set_position)),
		"rotation", sol::property(&Transform::get_rotation,
								  sol::resolve<void(const glm::vec3&)>(&Transform::set_rotation)),
		"scale", sol::property(&Transform::get_scale,
							   sol::resolve<void(const glm::vec3&)>(&Transform::set_scale)),
		"eulerAngles", sol::readonly_property(&Transform::get_euler_angles),
		"translate", sol::resolve<void(const glm::vec3&)>(&Transform::translate),
		"rotate", sol::overload(
			sol::resolve<void(const glm::vec3&)>(&Transform::rotate),
			sol::resolve<void(const float&, const float&, const float&)>(&Transform::rotate))
	);

	lua.new_usertype<MeshRenderer>(
		"MeshRenderer",
		"new", sol::no_constructor,
		"mesh", sol::property(&MeshRenderer::get_mesh, &MeshRenderer::set_mesh),
		"material", sol::property(&MeshRenderer::get_material, &MeshRenderer::set_material),
		"visible", sol::property(&MeshRenderer::is_visible, &MeshRenderer::display)
	);

	return true;
}

bool Scripting::bind_systems() {
	//Console binds...

	//auto test = [](std::string path, std::string id) -> Texture* {
	//	return resources.load_texture(path, id);
	//};

	

	lua.set_function("print", &Scripting::lua_print, this);
	
	sol::table resource_table = lua.create_named_table("Resources");

	resource_table.set_function("loadTexture", sol::resolve<Texture*(std::string, std::string)>(&Resources::load_texture), &resources);
	resource_table.set_function("loadShader", &Resources::load_shader, &resources);
	//resource_table.set_function("LoadTexture", test);
	resource_table.set_function("loadMesh", &Resources::fetch_mesh, &resources);
	resource_table.set_function("makeMaterial", &Resources::make_material, &resources);
	
	sol::table world_table = lua.create_named_table("World");
	world_table.set_function("createEntity", &World::create_entity, &world);
	world_table.set_function("time", &World::get_time, &world);

	return true;
}

void Scripting::call_lua_function(cstring name) {
	if (!okay) return;
	try {
		lua[name]();
	}
	catch (sol::error e) {
		console.print(e.what());
	}
	//sol::protected_function_result result = lua[name]();
	//if (!result.valid()) {
	//	sol::error e = result;
	//	console.print(e.what());
	//}
}

void Scripting::call_lua_function_with_args(cstring name, std::vector<cstring> argv) {
	if (!okay) return;
	try {
		lua[name](sol::as_args(argv));
	}
	catch (sol::error e) {
		console.print(e.what());
	}
	//sol::function_result result = lua[name](sol::as_args(argv));
	//if (!result.valid()) {
	//	sol::error e = result;
	//	console.print(e.what());
	//}
}

bool Scripting::load_start_script() {

	//Passing in a custom error handling function seems to prevent sol from throwing exceptions that can
	//just be returned as a sol::error from protected_function_result...
	//
	//This way of loading script files into the lua state seems to work for now, without the need to use
	//try/catch.

	//@Todo: Return protected_function_result and use to print error rather than passing in a custom handler,
	//		 use sol::script_pass_on_error instead.

	sol::protected_function_result result = lua.safe_script_file("res/start.lua", sol::script_pass_on_error);
	if (result.valid()) {
		okay = true;
		return true;
	}
	else {
		sol::error e = result;
		console.print(e.what());
		return false;
	}
}

void Scripting::clean_exit() {
	
}

