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

ConsoleCommand(resize) {
	if (argc != 2) {
		*system.console << "Usage: resize <width> <height>\n";
		return;
	}
	cstring arg0 = argv;
	cstring arg1 = dcf::str_find(argv, ' ') + 1;
	dcf::str_split(argv, ' ');

	int width = atoi(arg0);
	int height = atoi(arg1);

	if (width < 320 || height < 180) {
		*system.console << "Cannot resize to a screen smaller than 320x180\n";
		return;
	}
	if (width > 1920 || height > 1080) {
		*system.console << "Cannot resize to a screen larger than 1920x1080\n";
		return;
	}

	system.console->set_variable("vid_width", arg0, true);
	system.console->set_variable("vid_height", arg1, true);
	system.screen->refresh();
}
