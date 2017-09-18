#ifndef DELICIOUS_SYSTEM_INTERFACE_H
#define DELICIOUS_SYSTEM_INTERFACE_H

class Console;
class Resources;
class Screen;
class Input;

struct System_Ref {
	Console* console;
	Resources* resources;
	Screen* screen;
	Input* input;
};

#endif