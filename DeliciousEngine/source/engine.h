#ifndef DELICIOUS_ENGINE_H
#define DELICIOUS_ENGINE_H

#include <unordered_map>
#include <typeindex>
#include <assert.h>

class Game;
class Module;

class DeliciousEngine {
public:
	int run(int argc, char** argv);
	~DeliciousEngine();

	template <typename T> T& get();
	template <typename T> T* find();

	void quit() { running = false; }
private:
	template <typename T> void add();

	bool load();

	void add_default_modules();

	std::unordered_map<std::type_index, Module*> modules;
	std::vector<std::type_index> module_load_order;

	bool running;
};

template <typename T>
T& DeliciousEngine::get() {
	assert(modules.find(typeid(T)) != modules.end());
	return *static_cast<T*>(modules[typeid(T)]);
}

template <typename T>
T* DeliciousEngine::find() {
	if (modules.find(typeid(T) != modules.end()))
		return static_cast<T*>(modules[typeid(T)]);
	else
		return nullptr;
}

template <typename T>
void DeliciousEngine::add() {
	assert(modules.find(typeid(T)) == modules.end());
	modules[typeid(T)] = new T(*this);
	module_load_order.emplace_back(typeid(T));
}

#endif