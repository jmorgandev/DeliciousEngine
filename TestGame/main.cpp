#include "delicious_engine.h"

class TestGame : public Game {
	bool load(DeliciousEngine& engine) override {

	}
};

int main(int argc, char** argv) {
	DeliciousEngine engine;
	engine.launch(TestGame());
}