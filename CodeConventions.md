General
-------
Use tabs that equal 4 spaces. 

Typically use trailing braces everywhere ( if, else, functions, classes, structures, typedefs, etc. )

```$xslt
if ( x ) {
}
```
The else statement starts on the same line as the last closing brace
```$xslt
if ( x ) {
} else { 
}
```

Pad parenthesized expressions with spaces

```$xslt
if ( x ) {
}
```
Instead of 
```$xslt
if (x) {
}
```
Also 

```$xslt
x = ( y * 0.5f );
```
Instead of
```$xslt
x = (y * 0.5f);
```

Use precision specification for floating point values unless there is an explicit need for a double.

```
float f = 0.5f;
```
Instead of
```
float f = 0.5;
```
And
```
float f = 1.0f;
```
Instead of
```
float f = 1.f;
```


Function names should start with an upper-case
```$xslt
void Function( );
```

In Multi-Word function names, each word starts with an upper-case
```$xslt
void FunctionDoesSomething( );
```

Variable names start with a lower-case character
```$xslt
float x;
```

In Multi-Word variable names, Capitalize the First letter in each new Word
```$xslt
float maxDistanceFrom;
```

typedef names use the same naming convention as variables, however they always end with "_t"
```$xslt
typedef int tick_t; 
```

struct names use the same naming convention as variables as well, and they also always end with "_t"
```$xslt
struct footstep_t; 
```

Enum names use the same naming convention as variables. They always end with "_t". The enum constants use all upper-case characters. Multiple words in these are separated with an underscore.
```$xslt
enum bone_t {
    BONE_HEAD,
    BONE_NECK,
    BONE_COLLARBONE,
    //...
}
```

Avoid using recursion functions, but if you do, end the Name of the function with "_r"
```$xslt
void WalkBSP_r( int node ); 
```
 
Defined names use all upper-case characters. Multiple words are separated with an underscore. 
```$xslt
#define SIDE_FRONT 		0
```

Use const whenever you can
Use:
```$xslt
const int *p; // pointer to const int
int * const p; // const pointer to int
const int * const p; // const pointer to const int
```


File Names
----------
Each Class should be in a separate source file unless it makes sense to group in several small classes into one. 

The filename should be the same as the class/namespace name. 







#END
*Derived from an idSoftware Convention Guide "CodeStyleConventions.doc". Former link was (ftp://ftp.idsoftware.com/idstuff/doom3/source/CodeStyleConventions.doc) but as of now, has been taken down. 
