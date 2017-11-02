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
	system.console->clear();
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

ConsoleCommand(quit) {
	system.console->write_variable("eng_running", false);
}

ConsoleCommand(toggleconsole) {
	system.console->display_toggle();
}

ConsoleCommand(resize) {
	Console& out = *system.console;
	if (argc == 2) {
		cstring arg0 = argv;
		cstring arg1 = dcf::str_find(argv, ' ') + 1;
		dcf::str_split(argv, ' ');

		int w = atoi(arg0);
		int h = atoi(arg1);
		
		//minimum res = 640 x 480

		if (w < 640 || w > 1920) {
			out << "resize: width must be between 640 and 1920\n";
		}
		else if (h < 480 || h > 1080) {
			out << "resize: height must be between 480 and 1080\n";
		}
		else {
			system.screen->resize(w, h);
		}
	}
	else {
		out << "Usage: resize <width> <height>\n";
	}	
}