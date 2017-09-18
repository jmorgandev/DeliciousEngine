#include "cmds.h"
#include "system_ref.h"
#include "console.h"
#include "screen.h"
#include "dcf.h"

#define ConsoleCommand(a) void cmd_##a(System_Ref system, char* argv, int argc)
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

ConsoleCommand(add) {
	if (argc != 2) {
		*system.console << "Usage: add <int> <int>\n";
		return;
	}
	cstring arg0 = argv;
	cstring arg1 = dcf::str_find(argv, ' ') + 1;
	dcf::str_split(argv, ' ');

	int a = atoi(arg0);
	int b = atoi(arg1);
	int result = a + b;
	*system.console << a << " + " << b << " = " << result << '\n';
}