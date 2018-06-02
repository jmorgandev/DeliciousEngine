#include "entity.h"

#include "console.h"

void Entity::set_script(sol::this_state ts, sol::table script) {
	if (lua_script.valid() && lua_script == script) return;
	sol::state_view state(ts);

	lua_script = script;
	lua_script["entity"] = this;

	if (!lua_script[sol::metatable_key].valid())
		lua_script[sol::metatable_key] = state.create_table_with("__index", lua_script["entity"]);



	load();
	begin();
}

void Entity::load() {
	if (lua_script && lua_script["load"].valid()) {
		try {
			lua_script["load"](lua_script);
		}
		catch (sol::error e) {
			console.print(e.what());
		}
	}
}
void Entity::begin() {
	if (lua_script && lua_script["begin"].valid()) {
		try {
			lua_script["begin"](lua_script);
		}
		catch (sol::error e) {
			console.print(e.what());
		}
	}
}
void Entity::update() {
	if (lua_script && lua_script["update"].valid()) {
		try {
			lua_script["update"](lua_script);
		}
		catch (sol::error e) {
			console.print(e.what());
		}
	}
}
void Entity::end() {
	if (lua_script && lua_script["end"].valid()) {
		try {
			lua_script["end"](lua_script);
		}
		catch (sol::error e) {
			console.print(e.what());
		}
	}
}