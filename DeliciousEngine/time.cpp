#include "time.h"
#include <SDL/SDL.h>

bool Time::init(System_Ref sys) {
	system = sys;
	return true;
}

void Time::start() {
}

void Time::tick() {

}

void Time::clean_exit() {
}
