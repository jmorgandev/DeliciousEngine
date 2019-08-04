#ifndef DELICIOUS_ENGINE_H
#define DELICIOUS_ENGINE_H

#include <unordered_map>
#include <typeindex>
#include <assert.h>

class Game;
class System;

class DeliciousEngine {
public:
	int run(int argc, char** argv);
	~DeliciousEngine();

	template <typename T> T& get();
private:
	template <typename T> void add();

	bool load();

	void add_default_systems();

	std::unordered_map<std::type_index, System*> systems;

	bool running;
};

template <typename T>
T& DeliciousEngine::get() {
	assert(systems.find(typeid(T)) != systems.end());
	return *static_cast<T*>(systems[typeid(T)]);
}

template <typename T>
void DeliciousEngine::add() {
	assert(systems.find(typeid(T)) == systems.end());
	systems[typeid(T)] = new T(*this);
}

#endif