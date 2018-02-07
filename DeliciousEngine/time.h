#ifndef DELICIOUS_TIME_H
#define DELICIOUS_TIME_H

#include "system_var.h"
#include "dtypes.h"

class Time {
public:
	bool init();
	void start();

	void tick();

	void clean_exit();
private:
};
extern Time* time;

#endif