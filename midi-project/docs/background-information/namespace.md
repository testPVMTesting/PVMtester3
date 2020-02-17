# C++ Namespaces

## Problem Statement

First year students always have to been explained that it's okay
to reuse the same name for local variables in different functions or methods.
Imagine if this were not the case: each time you'd have
to make sure the name you chose is unique across potentially
thousands of lines of code.

Fortunately, names are *scoped*, i.e., the scope of an identifier
is the part of the code in which the identifier 'lives'.
You can reuse the same identifier as long as their scopes don't overlap.
If there is overlap, there are certain rules that determine
what the identifier refers to.

For example,

```cpp
int x;

struct Foo
{
    int x;

    void foo()
    {
        int x;
    }
}
```

There are three variables, all bound to the same identifier `x`.
Their scopes also overlap. Inside `foo`, the following rules apply:

* Using `x` refers to the local variable.
* To access the field, one can use `this->x`.
* The global variable is reachable through `::x`.

Type identifiers are more problematic though: by default, `struct`s and
`class`es are all defined in the global scope, which means
their scope always overlap.

```cpp
class Foo { ... };
class Bar { ... };
class Baz { ... };
class Qux { ... };
```

Name clashes form a potential problem: you need to take care
to assign a unique name to each of your types.
When working in large teams or with external code (i.e., libraries),
this can become a serious issue.
Note that function are less likely to suffer from this problem thanks to overloading.

## Solution

Enter namespaces. A namespace is similar to Java's packages:
one can define multiple classes with the same name,
as long as they are in different namespaces. However, unlike Java's packages,
C++'s namespaces are not in any way related to directories: any file can
contain functions/classes/... and put them in any namespace it fancies.

The syntax is as follows:

```cpp
// Header file
namespace my_namespace
{
    void my_function();

    struct my_type
    {
        void my_method();
    };
}

// CPP file
void my_namespace::my_function() { ... }

void my_namespace::my_type::my_method() { ... }
```

To refer to something in a namespace, you simply prefix it with `ns::` where
`ns` is the name of the namespace. An example you already encountered
is the `std` namespace: `std::cout`, `std::vector<int>`, etc.
