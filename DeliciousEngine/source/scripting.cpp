#include "scripting.h"

#include <functional>

#include <vec3.hpp>
#include <gtc/quaternion.hpp>
#include <gtx/string_cast.hpp>
#include <mat4x4.hpp>

#include "console.h"

static auto mult_overloads = sol::overload(
	[](const glm::vec3& v1, const glm::vec3& v2) -> glm::vec3 { return v1 * v2; },
	[](const glm::vec3& v1, const float& f) -> glm::vec3 { return v1 * f; },
	[](const float& f, glm::vec3& v1) -> glm::vec3 { return f * v1; }
);

static auto add_overloads = sol::overload(
	[](const glm::vec3& v1, const glm::vec3& v2) -> glm::vec3 {return v1 + v2; }
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
	lua.open_libraries();

	if (!bind_datatypes()) return false;
	if (!bind_systems()) return false;
	load_start_script();

	

	return true;
}

bool Scripting::bind_datatypes() {
	//register glm
	lua.new_usertype<glm::vec3>(
		"vec3",
		sol::constructors<glm::vec3(), glm::vec3(float), glm::vec3(float, float, float)>(),
		"x", &glm::vec3::x,
		"y", &glm::vec3::y,
		"z", &glm::vec3::z,
		sol::meta_function::addition, add_overloads
		//sol::meta_function::length, [](const glm::vec3& v1) -> int { return 3; }
	);
	//
	//lua.new_usertype<glm::quat>(
	//	
	//)
	return true;
}

bool Scripting::bind_components() {
	return true;
}

bool Scripting::bind_systems() {
	//Console binds...

	//lua.set_function("print", &Console::lua_print, &console);

	//sol::table console_bind = lua.create_named_table("Console");
	//console_bind.set_function("Print", &Console::lua_print, &console);

	//Resource binds...
	
	//sol::table resource_binds = lua.create_named_table("Resources");

	//
	//
	//

	return true;
}

void Scripting::call_lua_function_with_args(cstring name, std::vector<cstring> argv) {
	sol::function_result result = lua[name](sol::as_args(argv));
	if (!result.valid()) {
		sol::error e = result;
		console.print(e.what());
	}
}

bool Scripting::load_start_script() {

	//Passing in a custom error handling function seems to prevent sol from throwing exceptions that can
	//just be returned as a sol::error from protected_function_result...
	//
	//This way of loading script files into the lua state seems to work for now, without the need to use
	//try/catch.

	lua.safe_script_file("res/start.lua", error_handle);

	return true;
}

void Scripting::clean_exit() {
	
}

