#ifndef DELICIOUS_PHYSICS_H
#define DELICIOUS_PHYSICS_H

#include "system_ref.h"

class Physics {
public:
	bool init(System_Ref sys);
private:
	System_Ref system;
};

#endif