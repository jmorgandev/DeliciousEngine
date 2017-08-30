#ifndef DELICIOUS_SYSTEM_INTERFACE_H
#define DELICIOUS_SYSTEM_INTERFACE_H

class Console;
class Resources;
class Screen;

struct System_Interface {
	Console* console;
	Resources* resources;
	Screen* screen;
};

#endif