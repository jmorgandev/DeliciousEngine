#ifndef DELICIOUS_SCRIPTING_H
#define DELICIOUS_SCRIPTING_H

#include <sol.hpp>

class Scripting {
public:
	bool init();
	void clean_exit();

	bool load_start_script();
private:
	sol::state lua;

	bool bind_systems();
	bool bind_components();
	bool bind_datatypes();
};
extern Scripting scripting;

#endif
