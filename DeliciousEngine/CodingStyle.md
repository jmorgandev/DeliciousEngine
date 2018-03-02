# Header Files
Typically every .cpp file should have an accompanying .h file. There are some exceptions to this, like the program entry point (main.cpp).

Header filenames should use `snake_case`.  e.g. "`mesh_data.h`" instead of "`MeshData.h`"

Prefer defining template and inline functions in the same file as their declarations. If these are part of a class, define them inside the class definition.
```cpp
template <typename T>
class A {
	...
	void method() {...}
	...
}
```
All header file should have `#define` guards. The format of the definition name should be `DELICIOUS_<FILENAME>_H`
```cpp
#ifndef DELICIOUS_CAMERA_H
#define DELICIOUS_CAMERA_H
...
#endif
```
## Order of include statements
`#include` statements should be placed at the top of the file and MUST follow this order:
1. Associated header file if the current file is a .cpp.
2. A blank line (To separate #1 or a `#define` guard)
3. C++ libraries (e.g. `iostream`)
4. C libraries (e.g. `math.h`)
5. Other third-party libraries (e.g. `glm.hpp` or `glew.h`)
6. A blank line
7. Delicious Engine headers (e.g. `console.h` or `material.h`)

```cpp
#include "console.h"

#include <string>	// C++ library
#include <math.h>	// C library
#include <imgui.h>	// Third-party library

#include "dstr.h"	// Delicious Engine headers
#include "cmds.h"

...
```
## Forward Declarations
Avoid using forward declarations and instead `#include` the necessary headers. Forward declarations are nice and can potentially result in shorter compile times but they generally introduce **friction** when trying to refactor or make a related change.
```cpp
struct Shader; // Bad -- Avoid forward declaration.
```
```cpp 
#include "shader.h" // Good -- Include the relevant header instead.
```
If an `#include` statement is causing circular dependencies or any other compiler issues that cannot be resolved with anything else, then Forward Declarations may be used only if the reason for their use is provided with a comment.
# Scoping
## Namespaces
Namespaces should only be used with .h & .cpp files that provide utility / helper functions that aren't strongly related to the Delicious Engine. Aside from this, namespaces defined in engine code are not allowed.

Namespace names should generally be short and/or abbreviated. Anonymous namespaces should **not** be declared (Favour using `static` instead for internal variables and functions).
```cpp
namespace delicious_common_functions {...} // Bad -- Avoid long/explicit names.
```
```cpp
namespace dcf {...} // Good -- Prefer short/abbreviated names.
```
Avoid bringing namespace functions or variables into another scope with `using` statements. **Always** prefer the named accessor.
```cpp
using namespace dcf; // Bad -- Avoid bringing the namespace into another scope.
```
```cpp
int a = dcf::make_integer(5); // Good -- Access namespace with it's name.
```
This rule can only be broken when accessing standard C++ nested namespaces in order reduce **friction**. In this case, `using` statements should **always** be declared in local scope.
```cpp
// Bad -- Never spend more time typing than thinking.
std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
```
```cpp
// Good -- Reduces amount of typing.
void foo() {
	using namespace std::chrono;
	time_point<system_clock> now = system_clock::now();
}
```
## Nonmember, Static Member, and Global Functions
Global functions should either be placed inside a namespace (As a utility function) or declared with `static` inside a source file. Globally scoped functions are not allowed.

Avoid using classes or structs to group static functions together, instead prefer using the style above. Static methods can be declared within classes *only* if it relates to instances of the class or the static data of the class.
```cpp
// Bad -- Avoid grouping internal functions into anonymous namespaces.
namespace {
	void start_frame() {...}
	void end_frame() {...}
}
```
```cpp
// Good -- Prefer declaring internal functions as static.
static void start_frame() {...}
static void end_frame() {...}
```

## Local Variables
Place local variables in the most local scope possible, and as close to its first use as possible. This makes it easy to see why the variable was declared and what it is going to be used for.
```cpp
int foo;
foo = get_next_foo(); // Bad -- Don't separate declaration and initilization.
```
```cpp
int foo = get_next_foo(); // Good -- Declare and initialize on the same line.
```
Variables to be used with `if` and `while` statements should be declared within those statements if possible. Though this isn't a strict rule and can only be followed in specific cases, it provides cleaner and more readable code. However, `for` statements **should always** follow this rule (Apart from rare edge cases).
```cpp
if (int* foo = get_foo()) {...} // Good -- Variable is assigned and condition is still checked.
...
int* foo = get_foo();
if (foo != nullptr) {...} // Essentially the same result as above.
```
```cpp
while(int* foo = get_next_foo()) {...} // Good -- Variable is assigned and scoped to loop.
...
int* foo = get_next_foo();
while(foo != nullptr) {
	...
	foo = get_next_foo(); // Also a valid style, but prefer above if possible.
}
```
```cpp
int i = 5;
for(; i < 10; i++) {...} // Bad -- Variable should be declared within statement when possible.
```
```cpp
for(int i = 5; i < 10; i++) {...} // Good -- Variable is assigned and scoped to the loop.
```
## Static and Global Variables
Objects with static storage duration should be avoided **unless** the object is a core engine subsystem (e.g. Console, Screen, etc...). These subsystems should also have explicit init and exit functions rather than relying on constructors and destructors. 

The only exception to this rule is within `main.cpp`, where the source file should be treated as it's own module.
```cpp
static int camera_id; // Bad -- Non subsystem globally scoped variables are not allowed.
```
Variables inside of a function or statement scope should **not** be declared as `static` for persistence, **always** prefer defining persistent variables inside of a class definition.
```cpp
void foo() {
	static float t = 0; // Bad -- Variables that require scope persistence should not be static.
	...
	t++;
}
```
Global variables are only permitted if they can be declared as `const` or `constexpr`.

```cpp
int task_id = 7; // Bad -- This should be const or constexpr.
```
```cpp
constexpr task_id = 7; // Good -- Constant is evaluated at compile time.
```
# Classes
## Constructors
Prefer doing the absolute minimum work required to initialize the object in a constructor. Function calls and procedures that could potentially throw and error **should be avoided**.
```cpp
foo::foo() {
	...
	// Bad -- This could cause an error with no way to explicitly signal it.
	local_file = ifstream("res/file.txt");
}
```
Instead prefer putting error-prone code inside methods that can be used to signal any potential errors with return values or pointer/reference out variables.
```cpp
foo::foo() {...}
bool foo::load_file(std::string path) {
	local_file = ifstream("res/file.txt");
	return local_file.is_open(); // Good -- Errors are caught and can be dealt with externally.
}
```
## Structs or Classes
Prefer using `struct` for grouping data together that doesn't have a specific functionality. Otherwise use `class` definitions if grouped data has specific functionality.