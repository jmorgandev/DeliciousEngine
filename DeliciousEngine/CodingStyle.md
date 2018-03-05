## Header files
Typically every `.cpp` file should have an accompanying `.h` file. There are some exceptions to this, like the program entry point (`main.cpp`).

Prefer defining template and inline functions in the same file as their declarations. If these are part of a class, define them inside the class definition.
```cpp
template <typename T>
class A {
    ...
    void method(T input) {...}
    ...
}
```
All header files should have `#define` guards. The format of the definition guard should be `DELICIOUS_<FILENAME>_H`
```cpp
#ifndef DELICIOUS_CAMERA_H
#define DELICIOUS_CAMERA_H
...
#endif
```
### Order of include statements
`#include` statements should be placed at the top of the file and should follow this order:
1. Associated header file if the current file is a `.cpp`
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

#include "dstr.h"	// Core engine headers
#include "dtypes.h"

...
```
### Forward declarations
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
Namespaces should only be used with `.h` and `.cpp` files that provide utility or helper functions that aren't strongly related to core engine code. Aside from this, namespaces defined in engine code are not allowed.

Anonymous namespaces should not be declared (Favour using `static` instead for internal variables and functions).

Avoid bringing namespace functions or variables into another scope with `using` statements. Always prefer the named context accessor.
```cpp
using namespace dcf; // Bad -- Avoid bringing the namespace into another scope.
```
```cpp
int a = dcf::make_integer(5); // Good -- Access namespace by context.
```
This rule can only be broken when accessing standard C++ nested namespaces in order reduce **friction**. In this case, `using` statements should always be declared in local scope.
```cpp
// Bad -- Never spend more time typing than thinking.
std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
```
```cpp
// Good -- Reduces amount of typing and keeps namespace in local scope.
void foo() {
    using namespace std::chrono;
    time_point<system_clock> now = system_clock::now();
}
```
### Non-member, static member, and global functions
Global functions should either be placed inside a namespace (As a utility function) or declared with `static` inside a source file. Globally scoped functions are not allowed.

Avoid using classes or structs to group static functions together, instead prefer using namespaces. Static methods can be declared within classes *only* if it relates to instances of the class or the static data of the class.
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
### Local variables
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
### Static and global variables
Objects with static storage duration should be avoided unless the object is a core engine subsystem (e.g. Console, Screen, etc...). These subsystems should also have explicit `init` and `exit` functions rather than relying on constructors and destructors. 

The only exception to this rule is within `main.cpp`, where the source file should be treated as it's own module.

Variables inside of a function or statement scope should not be declared as `static` for persistence, always prefer defining persistent variables inside of a class definition.
```cpp
void foo() {
    static float t = 0; // Bad -- Variables that require scope persistence should not be static.
    ...
    t += 0.01f;
}
```
Global variables are only permitted if they can be declared as `const` or `constexpr`.
```cpp
int task_id = 7; // Bad -- This should be const or constexpr.
```
```cpp
constexpr task_id = 7; // Good -- Constant is evaluated at compile time.
```
## Classes
### Constructors
Prefer doing the absolute minimum work required to initialize the object in a constructor. Function calls and procedures that could potentially throw and error should be avoided in any constructor.
```cpp
foo::foo() {
    ...
    // Bad -- This could cause an error with no way to externally handle it.
    local_file = ifstream("res/file.txt");
}
```
Instead prefer putting error-prone code inside methods that can be used to signal any potential errors with return values or pointer/reference out variables.
```cpp
foo::foo() {...}
bool foo::load_file(string path) {
    local_file = ifstream("res/file.txt");
    return local_file.is_open(); // Good -- Errors are caught and can be dealt with externally.
}
```
An alternative solution would be to use a Builder pattern which provides a form of modular constructor that can be error checked before the object is finalized.
### Structs or classes
Prefer using structs for grouping data together that doesn't operate on itself. Otherwise use `class` definitions if grouped data needs to operate on itself.
```cpp
// Good -- struct is just a container for data
struct ImageData {
    vector<int> pixels;
    int width;
    int height;
};
```
```cpp
// Good -- Class has data but also methods to operate on that data.
class Image {
public:
    flip() {...}
    invert_color() {...}
private:
    vector<int> pixels;
}
```
### Inheritance
Prefer composition over inheritance, but typically any inheritance must be public.

Inheritance should be used sparingly and should generally only be 2 layers deep at a maximum (e.g. Interface > Base > Child). If your solution uses deeper inheritance than this, consider restructuring the hierarchy.
### Multiple inheritance
Multiple inheritance should be used in very rare cases, and only should be used with interfaces. If you are using multiple inheritance and one of the inherited classes is not an interface, then this will violate the rules of this style.
Ideally multiple inheritance should be used in rare cases to get a solution working, but it must never be present in release code unless there is a very good reason for it (Which must be outlined with comments).
### Interfaces
Interfaces are allowed, but prefer to use them sparingly. Do not haphazardly add interfaces without first considering their use case. As a general rule of thumb, interfaces should be used if 3 or more classes need to be accessed in a similar way.
### Operator overloading
Only define operator overloads if the functionality and result is closely related to the original operator. In other words, do not create new syntax (Like treating bitwise OR '|' as a unix pipe) that has to be explained.
### Access control
Make members `private` and only use `protected` if certain members must be inherited. `public` member variables are not allowed unless they are `static const`.
### Declaration order 
Member and method declarations in class definitions should be grouped into **single** sections of `public`, `protected`, and `private` in that order. Do not declare empty sections.

Generally prefer the following order of declarations:
1. Constructors
2. Destructor
3. Public methods
4. Protected member variables
5. Protected methods
6. Private member variables
7. Private methods

If a constructor or destructor must be non-public, prefer placing them at the top of the `protected` or `private` section.

When declaring `structs` or `enums` within the class, prefer placing them just before the member variable or method that uses them, regardless of what access section they are in.

Nested class definitions are not allowed.
## Functions
### Output parameters
Prefer using return values rather than output parameters. If you must use output parameters then place them after all other input parameters.
```cpp
// Bad -- Place output parameters after inputs.
void make_foo(foo* out, int in) {...}
```
```cpp
// Good -- Input and output parameters are left to right.
void make_foo(int in, foo* out) {...}
```
```cpp
// Good -- Prefer using return rather than output parameters.
foo* make_foo(int in) {...}
```
### Function length
Prefer writing short functions. Long functions are still allowed, but run the risk of causing **friction**.
```cpp
// Bad -- Consider breaking this function up into smaller ones.
void process_entire_frame() {
    ... // ~100 lines of code
}
```
```cpp
// Good -- Function is broken up into stages
void process_entire_frame() {
    prepare_gui();
    draw_world();
    draw_debug();
    draw_gui();
    swap_frame();
}
```
When breaking up a long function into shorter ones, generally do so if the shorter function can be re-used elsewhere. If a long function is broken up too much, then it can be difficult to immediately see what the code is doing.

If there is no way to break a long function up without causing readability issues, then it is preferred to use anonymous scopes (or unnamed scopes) to split the code up into readable sections.
```cpp
// Good -- Code is split up into readable sections
bool create_window() {
    /* start sdl */ { ... }
    /* create_window_from_flags */ {...}
    /* start_glew */ {...}
    ...
    return true;
}
```
### Function overloading
Prefer to overload functions rather than giving different names to different sets of parameters that perform the same operation. Overloaded functions must closely relate to each other in terms of functionality and the result of the procedure.
```cpp
// Bad -- Overloaded functions have different results.
int   calculate(int a, int b) { return a*b; }
float calculate(float a, float b) { return a/b; }
```
```cpp
// Bad -- Prefer overloading similar functions.
int   multiply_int(int a, int b) { return a*b; }
float multiply_float(float a, float b) { return a*b; }
```
```cpp
// Good - Overloaded functions related closely to each other.
int   multiply(int a, int b) { return a*b; }
float multiply(float a, float b) { return a*b; }
```
### Default arguments
Default arguments should be used when multiple function/method calls use the same parameters. Default arguments should not be used on `virtual` functions or when the default value might evaluate differently between different calls.
```cpp
// Bad -- Evaluates differently between calls.
void foo(int num = bar++) {...} 
```
### Trailing return types
C++ 11 introduced the trailing return type that works like the trailing return type seen in lambda expressions by using the `auto` keyword.
```cpp
// These functions are equivalent.
int foo(int num) { return num * 3; }
auto foo(int num) -> int { return num * 3; }
```
Prefer writing function signatures in the original C style unless the return type is easier to specify and read using the C++ 11 syntax.
```cpp
// Context heavy type information clutters code.
Material::UniformMeta Material::uniform_meta(string name) {...}
```
```cpp
// Trailing return type already has context.
auto Material::uniform_meta(string name) -> UniformMeta {...}
```
## Other C++ Features
### Friend
Use `friend` classes sparingly. They should only be used if two classes have a strong relationship but cannot be represented by inheritance or composition.
### Exceptions
Avoid introducing exceptions into the core engine code. Exceptions should only be used if a third-party library requires their use and provides no alternative.

Errors should generally be handled explicitly and purposefully in order to maintain the state of the program during runtime.

Whilst there are lots of arguments for and against exceptions, explicit error handling is generally much faster and typically results in less code, which is why this decision has been made for this style.
### Noexcept
Use the `noexcept` specifier when you are confident that third-party code that has the potential to throw exceptions will not throw in any potential use case by the core engine code. For this reason using `noexcept` should be rare and only used when third-party exceptions are causing performance overhead.
### RTTI
Avoid using run time type information for the core engine code. Consider restructuring class hierarchies or rethink your solution before employing the use of RTTI. 

If your code needs to perform differently depending upon specific child classes, then this should generally be implemented in the form of virtual methods or a double-dispatch solution.
### Casting
Prefer to use C style casting syntax when performing a **conversion** between primitive types and classes. Any other casting should be performed with C++ casting syntax.
```cpp
// Good -- Use C style syntax for conversions.
float bar = 5.3f;
int foo = (int)bar;
```
```cpp
// Bad -- Don't use C style syntax for type casting.
float* foo = new float(5.0f);
void* bar = (void*)foo; 
```
```cpp
// Good -- Use C++ style syntax for type casting.
float* foo = new float(2.7f)
void* bar = reinterpret_cast<void*>(foo);
```
### Streams
Avoid using streams for non-trivial code. Streams should only be used in simple cases (e.g. file io)
### Incrementing and decrementing
Prefer using post-increment (`i++`) and post-decrement (`i--`) operations to increase the readability of the code.

Pre-increment (`++i`) and pre-decrement (`--i`) operations should only be used when the incremented or decremented type has an expensive copy constructor.
### Const and constexpr
Prefer using `const` for constant literal variables and using `constexpr` for compile-time expressions, functions, or methods. `contexpr` metaprogramming is allowed and should be preferred over template metaprogramming.
### Integer sizes
Prefer using the default `int` specifier over any specific sizes (`int8`, `int16`, `int32`, etc). This lets the compiler determine the best size for the target processor architecture. Only use specific integer types if low-level byte and bit manipulation is needed in order to give a clearer idea of what manipulation is occurring.

Prefer using `unsigned int` (Or `uint` found in `dtypes.h`) to declare an integer variable that is not expected to be negative.
### Preprocessor macros
Generally only use `#define` for defining header guards and conditions that prevent or enable the compilation of certain code blocks (e.g. whether scripting should be built or not).

Prefer using `const` or `constexpr` to define constant literals.
### 0, nullptr, and NULL
Generally use the following style for assigning zero/invalid values to these types:
```cpp
int i = 0;
float f = 0.0f;
char c = '\0'; // Prefer over NULL
int* p = nullptr; // Prefer over NULL
```
Only use `NULL` if a third-party library uses it for error handling, function parameters, etc...
### Auto
Only use the `auto` specifier when declaring and assigning a variable in which the assigned value's type can easily be determined.
```cpp
// Bad -- Variable is assigned with a non-obvious type.
auto foo = get_last_value();
```
```cpp
// Good -- Variable is assigned with an integer literal.
auto foo = 182;
```
```cpp
// Good -- Variable type can be determined from function name.
auto foo = make_vector3(3.0f, 2.7f, 1.0f);
```
### Initializer lists
Initializer lists can be used but prefer to use constructors for classes.
```cpp
// Okay -- Constructor can be called with initializer list.
Vector3 vec = {3.0f, 2.7f, 1.0f};
```
```cpp
// Good -- Prefer to call the constructor directly.
Vector3 vec = Vector3(3.0f, 2.7f, 1.0f);
```
### Lambda expressions
Generally lambda expressions should only be used in combination with STL, but they can be used for situations where trivial function definitions can be avoided by declaring a lambda.
### Template metaprogramming
Avoid template metaprogramming wherever possible unless it can potentially provide an elegant solution to a non-trivial problem.
## Naming convention
### Variable names
Prefer variable names to use `snake_case`. Be reasonably descriptive and avoid abbreviation unless they are well known or unambiguous. Additionally, abbreviations are also accepted if the variable type has specific uses which provide the context lost from abbreviating. 
```cpp
int n; // Bad -- Provides no meaning or context.
int n_conn; // Bad -- Ambiguous abbreviation.
int pc; // Bad -- "pc" has multiple meanings.
```
```cpp
int object_count; // Good -- No abbreviation.
int num_errors; // Good -- "num" is a common abbreviation.
Camera* cam; // Good -- Abbreviated but type provides context.
MeshData md; // Good -- Abbreviated but type provides context.
```
Ultimately, these rules can be flexed under specific circumstances, such as defining local temporary variables or if a name only makes sense within the context of where it is declared (e.g. in a class). Generally follow the above guide, but bend these rules within reason.

Variable names with underscores from `snake_case` can also drop the underscore if the result does not suffer a large degree of readability and still makes sense.
```cpp
// Bad -- Sacrifices too much readability.
float farclipplane = 100.0f;
```
```cpp
float far_clip_plane = 100.0f; // Good -- Separates all words.
float farclip_plane = 100.0f; // Good -- Separates unrelated words.
```
### Function names
Function names should also generally follow the same rules as variable naming. Use `snake_case` and be descriptive, but not too descriptive.
```cpp
void do_camera() {...} // Bad -- Non-obvious purpose.
int checkError(int code) {...} // Bad -- Doesn't use snake_case.
```
```cpp
void move_camera() {...} // Good -- Name gives purpose and context.
int check_error(int code) {...} // Good -- Correctly uses snake_case.
```
### File names
Filenames should also follow the `snake_case` style. C++ source files should end in `.cpp` and header files should end in `.h` (**not** `.hpp`). In the rare case that a source file must be explicitly included, it must end in `.inc` instead.

Filenames themselves should be descriptive, but not overly specific. For example, "`entity.h`" is ambiguous, but the core engine code has only one use for a class/module relating to an "entity". In contrast, "`game_entity.h`" would be more descriptive, but is not required considering that there is only one concept of an "entity" in the core engine code.
### Type names
Type names should strictly use `UpperCamelCase` in order to contrast with the variable/function naming of `snake_case` to provide clear distinction between types and variables/functions.

The names of all types -- classes, structs, typedefs, aliases, and templates -- should have this same naming convention.
```cpp
// Good -- Type definitions all use UpperCamelCase.
class ScriptedEntity {...};
struct ImageData {...};
enum class ResourceType {...};
```
### Class and struct members
Member variables of classes and structs should be named exactly like ordinary variables and functions (As outlined above). 
```cpp
class Sound {
public:
    void play() {...}
    void play_reversed() {...}
    void play_pitched(float pitch) {...}
private:
    string name;
    float pitch;   
};
```
Avoid using any type of unique identifier for member variables like "`name_`" or "`m_name`". If you want to name class methods after member variables, consider using different names or prefixing the method name with "`get_`" or "`set_`" if you are writing getter and setter methods.

### Const and constexpr names
Prefer using `UPPER_SNAKE_CASE` to name `const` or `constexpr` variables. 
```cpp
const int MAX_SOUNDS = 15;
constexpr MAX_TEXTURES = 15;
```
However when writing `constexpr` functions or methods, prefer using the same naming convention as normal functions and methods.
```cpp
constexpr int factorial(int n) {...}
```
### Namespace names
Namespace names should generally be short or abbreviated. 
```cpp
// Bad -- Avoid long/explicit names.
namespace delicious_common_functions {...}
```
```cpp
// Good -- Prefer short/abbreviated names.
namespace dcf {...}
```
### Enum names
Prefer naming enums as if they were types (Using `UpperCamelCase`), but the individual enumerators should be named as if they were constant variables (using `UPPER_SNAKE_CASE`).
```cpp
enum MeshAttributes {
    VERTICES,
    NORMALS,
    COLORS,
    ATTRIBUTE_COUNT
};
```
### Macro names
If you really need to define a macro, generally use the same naming style as enums and constant variables.
```cpp
#define MAX(x) ...
#define BUFFER_SIZE 0xFF
```
## Comments
### Syntax
Prefer using `//` over `/* */` but either syntax is fine. Multiline comments should always be preceded and followed by blank lines in the same syntax.
```cpp
// This is a single line comment.

/* 
 * This is a multiline comment. Be sure add an aligned asterisk
 * to each new line in the comment block, including the end.
 */

//
// This is also a multline comment, which follows the same rules
// as the previous multiline comment.
//
```
### Comment tips
There are no solid rules on commenting in this style. Generally be clear and concise, avoid unnecessary explanations (don't state the obvious), and avoid using too little or too many comments in a code block.

As a rule of thumb, comments should be used for any non-trivial procedure. Also try to follow the "Linux kernel" commenting style, where comments should describe **what** is happening and not **why** or **how** it is happening, although this is not required.
### @ statements
Use of @ is useful in certain comments to describe future actions or considerations for a code block.

Use `//@Todo: ...` to describe an action that should happen in the future. This can be an unimplemented feature or any changes to the code that should be made. 
```cpp
...
else {
    //@Todo: Print an error to the console when this happens.
}
```
Use `//@Temp: ...` to describe a temporary code block.  This doesn't have to have a full comment, but can be useful to describe why it is there.
```cpp
//@Temp: Should be iterating through a container, hard-coded
// for now.
```
Use `//@Speed: ...` to describe a potential code improvement relating to performance optimizations.
```cpp
//@Speed: Consider SoA vs Aos, data-oriented design.
```
Use `//@Memory: ...` to describe a potential code improvement relating to memory optimizations.
```cpp
//@Memory: Don't store the same buffer on the CPU and GPU.
```
Use `//@Bug(num): ...` to describe a labelled bug that has been discovered but not fixed.
```cpp
//@Bug(16): Sometimes throws null-reference exception.
```
Use `//@Deprecated: ...` to describe a function, variable, or code block that should no longer be in used. Generally also include what should be used instead.
```cpp
//@Deprecated: Use load_texture(path, id) instead, which uses
// stb_image instead of SDL_Image.
```
Any @ statement can be extended with parenthesis in order to provide context. This helps categorize @ statements for external tools. If you want to provide context when using the @Bug statement, place a comma after the bug id. This is not encouraged though, as the bug list should already provide more context than the @ statement.
```cpp
//@Todo(input): Add user defined buttons.
//@Speed(opengl): Use instanced rendering instead.
//@Bug(11, scripts): Script isn't loading correctly.
```
## Formatting
### Line length
Generally try to code into 120 character long lines before splitting them, this is so screen resolutions as low as 1366 x 768 (Incidentally, my laptop's resolution) can still fit code in an IDE like Visual Studio without scrolling horizontally or line wrapping.
### Spaces or tabs?
Use 4 spaces per indentation. Prefer setting up your IDE to use smart-tabs which insert 4 spaces when pressing <kbd>tab</kbd>, and remove 4 spaces when pressing <kbd>shift + tab</kbd>. **Don't use tab characters**.
### Curly braces and new lines
Prefer placing the first curly brace on the same line as it's declaration.
```cpp
void foo() {
}
class Bar {
};
struct Data {
};
```
`if` statements and similar control flow statements should follow the same rules. If an there needs to be an `else` statement, prefer placing it below the final curly brace of the previous `if` statement.
```cpp
if (condition) {
}
else {
}

while (condition) {
}
```
The only exception to this rule is the `do while` loop, which should have the `while` statement placed on the same line as the closing curly brace.
```cpp
do {
} while(condition);
```
Single line control statements (Without curly braces) are permitted, and can use any style so long as the style is consistent throughout the code block.
```cpp
// Bad -- Don't mix and match styles.
if (condition) 
    do_something();
else do_something_else();
for (condition) do_something();
while (condition)
    do_something(); 
```
```cpp
// Good -- Everything is on the same line.
if (condition) do_something();
else do_something_else();
for (condition) do_something();
while (condition) do_something();
```
```cpp
// Good -- Separate control flow statements from executed code.
if (condition)
    do_something();
else
    do_something_else();
for (condition)
    do_something();
while (condition)
    do_something();   
```
### Function calls
Prefer calling functions and passing parameters on the same line if it is possible to do so without breaking the line-width rule. If this isn't possible, then arguments can be placed on a new line so long as the indentation is aligned to the **first** argument.
```cpp
// Bad -- Long parameter list breaks the line-width rule.
handle = CreateWindowEx(WS_EX_CLIENTEDGE, winclass, wintitle, winstyle, ...
```
```cpp
// Bad -- Function call should not look like a scope.
handle = CreateWindowEx(
    WS_EX_CLIENTEDGE,
    winclass, wintitle, 
    winstyle,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    WINDOW_WIDTH,
    WINDOW_HEIGHT,
    NULL, NULL,
    hinstance, NULL
);
```
```cpp
// Good -- Long parameter list is aligned with first argument.
handle = CreateWindowEx(WS_EX_CLIENTEDGE,
                        winclass,
                        wintitle,
                        winstyle,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        WINDOW_WIDTH,
                        WINDOW_HEIGHT,
                        NULL, NULL,
                        hinstance, NULL);
```
### Declaring pointers and references
The additional syntax required to declare a variable as a pointer or reference should always be next to the **type** and not the name.
```cpp
// Bad -- Syntax is placed next to the name rather than the type.
int *foo = new int(15);
int &bar = *foo;
int *baz = &bar;
```
This is because the format above has the same way of writing `*foo` or `&bar` whilst having two different meanings.
```cpp
// Good -- Syntax is placed next the types.
int* foo = new int(15);
int& bar = *foo;
int* baz = &bar;
```
### Preprocessor directives
Any preprocessor directive should be placed at the beginning of each line regardless of the current indentation.
```cpp
// Bad -- Preprocessor directives must stand out from ordinary code.
void foo() {
    #if USE_HEAP
    float* inputs = new float[16];
    #else
    ...
    #endif
}
```
```cpp
// Good - Preprocessor statements are separated from code.
void foo() {
#if USE_HEAP
    float* inputs = new float[16];
#else
    ...
#endif     
}
```
### Something not mentioned
Always compare the code you write to the existing source code of the engine. If you keep the style consistent and it looks like it matches the existing code, chances are you are following the style.

Generally things that are missing are minor details that can be followed just by looking at the existing source code.
