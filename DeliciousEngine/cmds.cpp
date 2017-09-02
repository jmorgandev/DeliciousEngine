#include "cmds.h"
#include "system_interface.h"
#include "console.h"

#define ConsoleCommand(a) void cmd_##a(System_Interface system, char* argv, int argc)
#define CMD(a) ConsoleCommand(a);
COMMAND_LIST
#undef CMD

#define CMD(a) {#a, cmd_##a, false},
std::vector<console_cmd> default_cmds = { COMMAND_LIST };
#undef CMD

//
// Default console command definitions
//
ConsoleCommand(clear) {
	system.console->clear_buffer();
}

