# The 'Delicious Engine'
A generalized 3D engine built for rapid-prototyping & game-jam participation. Currently in development.

## Background
Previous 3D engines I have built in C++ have used the fixed function pipeline of OpenGL (2.1) and only have minimal aspects of data-oriented design within them. This is reasonable for short projects however proves unmaintainable for any future modification and discourages the use of the engine to develop any type of game it wasn't intended to be used for.

In the past, I have attempted to create an ECS (Entity-Component-System) based game engine, however this resulted in a large undertaking which I ended up putting on hold. In the end if I were to develop an engine similar to Unity or Unreal, then why not just use Unity or Unreal?

Although I have plenty of experience developing in both Unity and Unreal, the rapid-prototyping nature of Unity seems to be slowing down with each new iteration in order to support more ambitious projects that AAA game studios participate in. Coupled with Unreal being built specifically for AAA game studios (Which typically leads to unnecessary features being automatically built in to a small project), I started to develop my own engine. Of which the focus would be towards small game projects, rapid-prototyping, and extensibility.

## Features
This engine is being developed with modern graphics techniques (OpenGL 4.3+), and being designed for general use rather than being tailored for any one type of game development. This includes the ability to develop a game/prototype without having to modify the engine source code, by using Lua and AngelScript as an interface to engine functionality as well as to specify game-object behaviour.

The engine comes with a development console to allow inspection of the engine without having manually debug the C++ source code in Visual Studio. Whilst this doesn't and shouldn't replace normal debugging, it does grant the ability to identify solutions to simple problems or logic errors.

The same way that the engine can be scripted with lua/angelscript, it can also be extended with lua/angelscript. If the engine doesn't support a file format (Which could be an asset, or a level), then the engine will subsequently search for a lua/angelscript file that tells the engine how to deal with that particular file format. As a result, the user can add support to any custom asset or level files that the engine doesn't know how to process.

## Technologies Used/Planned
- OpenGL 4.3+
- glew
- SDL2
- stb_image
- SDL2_mixer (May change to FMod)
- Lua 5.3+
- Python 3+

## Development Information

### Current Version (0.0.8)
- Added ImGui Library (For "immediate-mode" style gui rendering)
- Changed the console to use ImGui
- Remove unnecessary gui renderers (BoxRenderer, FontRenderer)
- Small systems refactor (Engine is no longer a class, but this can be reverted)
- Added stb_* libraries
- Added support for stb_image in resource system
- Restructured project directories
- Added coding style document to reflect the style of the source code

### Next Milestone (0.1.0)
- [ ] Embedded scripting
- [x] Game-object management
- [ ] Scripting entry point
- [x] Moving camera test

Last updated: 07/03/2018
