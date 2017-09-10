# Spartan Code Style Guidelines

Table of Contents

1. [General](#general)
2. [Spaces](#spaces)
3. [Braces](#braces)
4. [Casting](#casting)
5. [Naming](#naming)
6. [Exceptions](#exceptions)
7. [Credits](#credits)
8. [IDE Support](#ide-support)

## General

Generally, all contributors should follow the [Google C++ Style Guidelines][c2]
in addition to the cases below. If any points listed below conflict with the
Google C++ Style Guidelines, the rules written below should be given priority.

## Spaces

Every level of indentation should be 4 spaces. Don't use tabs.

Typically, a space is inserted before parentheses in `if`, `while`, `for`, `switch`,
`catch` etc.

```cpp
if ( statement ) {
}
```

Parenthesized expressions should also be padded with spaces, like this:

```cpp
if ( statement ) {
}
```

Variables should also follow this rule:

```cpp
float x = ( y * 0.5f );
```

Exceptions to these space rules are castings and no parameter functions.

```cpp
float f = (float) x * y;
float f = ( float ) x * y; // Don't do this

void Function() {
}

// v  Don't do this  v
void Function( ) {
}
// ^  Don't do this  ^
```

## Braces

Don't insert an additional newline after braces.

```cpp
void Function() {
}

// v  Don't do this  v
void Function()
{
}
// ^  Don't do this  ^
```

This also applies to `if`, `else`, `while`, `for`, `switch`, `catch` etc.

## Casting

For floating point values, explicitly specify it unless there is a explicit need for a double.

```cpp
float f = 1.0f;
float f = 1.0; // Don't do this
float f = 1.f; // Don't do this
```

If not something different is explicitly required, always cast using `(type)`.

```cpp
float f = (float) x * y;
float f = static_cast<float>( x * y ); // Only do this when required
float f = dynamic_cast<float>( x * y ); // Only do this when required
float f = const_cast<float>( x * y ); // Only do this when required
float f = reinterpret_cast<float>( x * y ); // Only do this when required
```

[Stackoverflow](https://stackoverflow.com/questions/332030/when-should-static-cast-dynamic-cast-const-cast-and-reinterpret-cast-be-used)
regarding this rule.

## Naming

Identifiers use only ASCII letters and digits, and, in a small number
of cases noted below, underscores.

Hungarian notation like `_name`, `iName`, `i_Name`, `EName` is not used.

#### Files

Files are named in all lowercase and don't use any underscores.

C++ source files should have the `.cpp` file extension and
header files should have the `.h` file extension.
With every C++ source file, a header file should exist (There are a few exceptions).

#### Namespaces

Namespaces follow [UpperCamelCase][ucc]:

```cpp
namespace Aimbot {
}

namespace MySpecialNamespace {
}
```

#### Types and Functions

Types and Functions follow [UpperCamelCase][ucc]:

```cpp
void Function() {
}

void MySpecialNamespace::MyFunction( float x ) {
}
```

### Variables and Function Parameters

Variables and Function Parameters follow [lowerCamelCase][lcc]:

```cpp
float maxDistance = 1.5f;

void SetMaxDistance( float maxDistance ) {
}
```

### Typedefs and Structs

Typedefs and Structs follow [lowerCamelCase][lcc], but are prefixed with `_t`:

```cpp
typedef int tick_t;
struct footstep_t;
```

### Constants and #define

Constants follow [lowerCamelCase][lcc] and #define constants follow `CONSTANT_CASE`:

```cpp
#define GLOBAL_SALT 0x5DEECE66DL

const char* myText = "Hello World!";
```

### Enums

Enum names follow [UpperCamelCase][ucc] and are not prefixed by anything.
The enum constants follow `CONSTANT_CASE`:

```cpp
enum Bone {
    BONE_HEAD,
    BONE_NECK,
    BONE_COLLARBONE
}
```

### Recursive Functions

Recursive functions should be avoided and follow [UpperCamelCase][ucc], but are prefixed with `_r`:

```cpp
void WalkBSP_r( int node );
```

## Exceptions

You may diverge from the rules when dealing with code that does
not conform to this style guide.

If you find yourself modifying code that was written to specifications
other than those presented by this guide, you may have to diverge from these
rules in order to stay consistent with the local conventions in that code.
If you are in doubt about how to do this, ask the original author or the person
currently responsible for the code. Remember that _consistency_ includes local
consistency, too.

## IDE Support

The repository contains a CLion Style File called `clion-spartan-code-style.xml`.
Import it into CLion via the Settings dialogue and apply it to the cloned Spartan project.

## Credit

This is a mix of IDSoftware's [Code Convention Guide][c1] and the [Google C++ Style Guidelines][c2].

[c1]: ftp://ftp.idsoftware.com/idstuff/doom3/source/CodeStyleConventions.doc
[c2]: https://google.github.io/styleguide/cppguide.html

[ucc]: https://google.github.io/styleguide/javaguide.html#s5.3-camel-case
[lcc]: https://google.github.io/styleguide/javaguide.html#s5.3-camel-case
[pc]: https://en.wikipedia.org/wiki/PascalCase
