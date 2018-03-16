#include "scripting.h"

#include <functional>

#include "console.h"

bool Scripting::init() {
	lua.open_libraries(sol::lib::base, sol::lib::package);

	if (!bind_systems()) return false;
	load_start_script();

	

	return true;
}

bool Scripting::bind_systems() {
	sol::table console_bind = lua.create_named_table("Console");
	console_bind.set_function("Print", &Console::print, &console);
	console_bind.set_function("RegisterCommand", &Console::register_lua_cmd, &console);
	

	return true;
}

bool Scripting::load_start_script() {
	auto script = lua.load_file("res/start.lua");
	if(script.valid()) script();
	else {
		
	}
	return true;
}

void Scripting::clean_exit() {
	
}

