#ifndef DELICIOUS_GAME_H
#define DELICIOUS_GAME_H

#include "delicious_engine.h"

class Game {
public:
	virtual bool load(DeliciousEngine& engine) = 0;
private:

};

#endif