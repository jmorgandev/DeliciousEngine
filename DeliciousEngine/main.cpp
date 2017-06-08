#include "engine.h"
#include "dtypes.h"

int main(char** argv, int argc) {
	Engine delicious_engine;

	if (delicious_engine.init(argv, argc)) {
		delicious_engine.run();
	}
	delicious_engine.clean();

	return 0;
}