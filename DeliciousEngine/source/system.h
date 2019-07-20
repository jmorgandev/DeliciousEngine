#ifndef DELICIOUS_SYSTEM_H
#define DELICIOUS_SYSTEM_H

class System {
public:
	virtual bool load() = 0;
	virtual bool start() { return true; }
	virtual void tick() {}
	virtual bool free() = 0;
};

#endif