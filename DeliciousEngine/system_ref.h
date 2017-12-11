#ifndef DELICIOUS_SYSTEM_INTERFACE_H
#define DELICIOUS_SYSTEM_INTERFACE_H

class Console;
class Resources;
class Screen;
class Input;
class World;
class Physics;
class Time;
//@TODO: Add physics?

struct System_Ref {
	Console*   console;
	Resources* resources;
	Screen*    screen;
	Input*     input;
	World*     world;
	Physics*   physics;
	Time*      time;
};

#endif