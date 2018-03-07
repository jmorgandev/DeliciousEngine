#ifndef DELICIOUS_CMDS_H
#define DELICIOUS_CMDS_H

#include "console_types.h"
#include <vector>
#include <array>


//@Todo: Deprecate this in favour of binding function pointers to objects with std::bind.
#define ConsoleCommand(a) static inline void cmd_##a(std::vector<cstring> args)

#endif