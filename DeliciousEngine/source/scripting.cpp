#include "scripting.h"

#include <functional>

#include "console.h"

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
	lua.open_libraries(sol::lib::package);

	if (!bind_systems()) return false;
	load_start_script();

	

	return true;
}

bool Scripting::bind_systems() {
	sol::table console_bind = lua.create_named_table("Console");
	console_bind.set_function("Print", &Console::lua_print, &console);	

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

