#ifndef DELICIOUS_WORLD_H
#define DELICIOUS_WORLD_H

#include "system_ref.h"

class World {
public:
	bool init(System_Ref sys);
	void clean_exit();

private:
	System_Ref system;
};

#endif