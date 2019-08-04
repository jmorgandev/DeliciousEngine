#ifndef DELICIOUS_SYSTEM_H
#define DELICIOUS_SYSTEM_H

class DeliciousEngine;
class System {
public:
	System(DeliciousEngine& engine_ref) : engine(engine_ref) {}
	virtual bool load() = 0;
	virtual bool start() { return true; }
	virtual void tick() {}
	virtual bool free() = 0;
protected:
	DeliciousEngine& engine;
};

#endif