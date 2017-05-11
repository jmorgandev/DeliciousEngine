#ifndef DELICIOUS_ENGINE_H
#define DELICIOUS_ENGINE_H

class Engine {
public:
	bool init(char** argv, int argc);
	void run();
	void clean();

private:
	bool running;
};

#endif