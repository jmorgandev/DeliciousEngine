#ifndef DELICIOUS_CONSOLE_TYPES_H
#define DELICIOUS_CONSOLE_TYPES_H

#include <functional>

#include "dtypes.h"
#include "system_var.h"
#include <vector>
#include "build_settings.h"

typedef unsigned int console_symbol;

struct ConsoleVariable {
	void* data;
	enum Type { BOOL, INT, FLOAT, SYM } type;
};

#endif