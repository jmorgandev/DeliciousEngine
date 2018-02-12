#ifndef DELICIOUS_SCRIPTING_H
#define DELICIOUS_SCRIPTING_H
#pragma once

#include <sol.hpp>

class Scripting {
public:
	bool init();
	void clean_exit();

private:

};
extern Scripting* scripting;

#endif
