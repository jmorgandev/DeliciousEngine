#ifndef DELICIOUS_CONSOLE_H
#define DELICIOUS_CONSOLE_H

class Engine;
class Console {
public:
	bool init(Engine* engine_in);

private:

	Engine* engine;
};

#endif