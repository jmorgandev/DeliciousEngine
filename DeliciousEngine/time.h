#ifndef DELICIOUS_TIME_H
#define DELICIOUS_TIME_H

#include "system_ref.h"
#include "system_var.h"
#include "dtypes.h"

class Time {
public:
	bool init(System_Ref sys);
	void start();

	void tick();

	void clean_exit();
private:
	System_Ref system;
};

#endif