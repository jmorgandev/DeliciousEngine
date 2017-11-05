#include "engine.h"
#include "dtypes.h"
#include <iostream>


int main(char** argv, int argc) {	
	Engine delicious_engine;

	if (delicious_engine.init(argv, argc)) {
		delicious_engine.run();
	}
	delicious_engine.shutdown();

	return 0;
}