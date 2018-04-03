#include "entity.h"

#include "console.h"

void Entity::set_script(sol::this_state ts, sol::table script) {
	//@Todo: use state_view to get access to create a metatable or something
	sol::state_view state(ts);
	lua_script = script;
	lua_script["entity"] = this;
	sol::table meta = lua_script[sol::metatable_key] = state.create_table_with(
		"__index", lua_script["entity"],
		"__newindex", [](sol::table t, sol::object k, sol::object v) {
			if (t["entity"][k].valid())
				t["entity"][k] = v;
			else
				t[k] = v;
		}
	);
	console.print("before load");
	load();
	begin();
}

void Entity::load() {
	if (lua_script["load"].valid()) {
		try {
			lua_script["load"](lua_script);
		}
		catch (sol::error e) {
			console.print(e.what());
		}
	}
}
void Entity::begin() {
	if (lua_script["begin"].valid()) {
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
	if (lua_script["end"].valid()) {
		try {
			lua_script["end"](lua_script);
		}
		catch (sol::error e) {
			console.print(e.what());
		}
	}
}