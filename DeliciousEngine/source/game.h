#ifndef DELICIOUS_GAME_H
#define DELICIOUS_GAME_H

class DeliciousEngine;
class Game {
public:
	virtual ~Game() = default;

	virtual bool run(int argc, char** argv);

	virtual bool startup(DeliciousEngine & engine_ref) { return true; }
	virtual void update() {}
	virtual bool shutdown() { return true; }
private:
	DeliciousEngine engine;
};

#endif