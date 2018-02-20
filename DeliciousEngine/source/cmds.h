#ifndef DELICIOUS_CMDS_H
#define DELICIOUS_CMDS_H

#include "console_types.h"
#include <vector>
#include <array>

#define ConsoleCommand(a) static inline void cmd_##a(std::vector<cstring> args)

#endif