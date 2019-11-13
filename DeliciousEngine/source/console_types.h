#ifndef DELICIOUS_CONSOLE_TYPES_H
#define DELICIOUS_CONSOLE_TYPES_H

#include <functional>

#include "dtypes.h"
#include <vector>

typedef unsigned int console_symbol;

struct ConsoleVariable {
	void* data;
	enum Type { BOOL, INT, FLOAT, SYM } type;
};

#endif