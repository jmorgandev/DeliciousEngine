#include "time.h"
#include "console.h"

bool Time::init(System_Ref sys) {
	system = sys;

	//system.console->register_variable("time_timestep")

	return true;
}

void Time::start() {
}

void Time::tick() {

}

void Time::clean_exit() {
}
