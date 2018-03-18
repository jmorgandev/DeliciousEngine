#ifndef DELICIOUS_SCRIPTING_H
#define DELICIOUS_SCRIPTING_H

#define SOL_CHECK_ARGUMENTS
#define SOL_SAFE_FUNCTIONS
#define SOL_SAFE_REFERENCES
#include <sol.hpp>

#include "dtypes.h"

class Scripting {
public:
	bool init();
	void clean_exit();

	void call_lua_function_with_args(cstring name, std::vector<cstring> args);

	bool load_start_script();
private:
	sol::state lua;

	bool bind_datatypes();
	bool bind_components();
	bool bind_systems();
};
extern Scripting scripting;

#endif
