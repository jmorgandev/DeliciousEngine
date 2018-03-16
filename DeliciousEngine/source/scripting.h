#ifndef DELICIOUS_SCRIPTING_H
#define DELICIOUS_SCRIPTING_H

#include <sol.hpp>

#include "dtypes.h"

class Scripting {
public:
	bool init();
	void clean_exit();

	void execute_lua(cstring code);
	void execute_lua_function(cstring name, std::vector<cstring> args) {
		lua[name](sol::as_args(args));
	}

	bool load_start_script();
private:
	sol::state lua;

	bool bind_systems();
	bool bind_components();
	bool bind_datatypes();
};
extern Scripting scripting;

#endif
