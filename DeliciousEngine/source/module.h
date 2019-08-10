#ifndef DELICIOUS_SYSTEM_H
#define DELICIOUS_SYSTEM_H

class DeliciousEngine;
class Module {
public:
	Module(DeliciousEngine& engine_ref) : engine(engine_ref) {}
	Module(Module const &) = delete;
	Module& operator=(Module const &) = delete;
	virtual ~Module() = default;

	virtual bool load() = 0;
	virtual bool start() { return true; }
	virtual void tick() {}
	virtual bool free() = 0;
protected:
	DeliciousEngine& engine;
};

#endif