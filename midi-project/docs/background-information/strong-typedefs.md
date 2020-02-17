# Strong Typedefs

**Disclaimer** A fairly long explanation follows during which some weird tricks
will be performed. Do not be intimidated by this: in the end, what you
will actually need to *do* is actually quite simple. The text below attempts
to make you *understand* what it is you're doing. We do not expect you
to be able to come up with this on your own: instead, the intention
is to give you a quick look at some more advanced programming techniques.

The purpose of this section is to break down your preconceptions
about types and classes and how you can use them, more specifically:

* How to better rely on the compiler to help us prevent bugs.
* How to define classes by building them out of large components instead of field by field and method by method.

## Premise

Say you're writing a `Person` class. Each `Person` object features a weight (kg) and a height (m). You pick `double` as type to represent them both.

```cpp
struct Person
{
    Person(double weight, double height)
        : weight(weight), height(height) { }

    double weight, height;
}
```

It is of course important that both values don't get mixed up. You wouldn't want to
accidentally create a 80m large giant weighing less than 2kg.
However, such a mistake would not be stand out in the code below:

```cpp
Person person(1.82, 80);
```

The compiler does not complain: the `Person` constructor requires two `double`s,
you give it two `double`s, so as far as the compiler is concerned,
this code is correct.

Now, what can we do to have the compiler help us catch this kind of mistake?
C# and Python, for example, let you mention the paramter name explicitly:

```python
# Python
class Person:
    def __init__(self, weight, height):
        self.weight = weight
        self.height = height

p = Person(height=1.82, weight=80)
```

```cs
// C#
class Person
{
    public Person(double weight, double height)
    {
        this.Weight = weight;
        this.Height = height;
    }

    public double Weight { get; set; }

    public double Height { get; set; }
}

var p = new Person(height: 1.82, weight: 80);
```

However, C++ does not support named parameters. We'll have to look
elsewhere for a solution.

## The Way Physicists Do It

Physicists have more or less the same issue: they don't want their
quantities to be mixed up. Their solution is to rely on units of measure, such
as meter, second, kilogram, etc. For example, it is forbidden
to add to quantities together with different units: 5m + 3s makes no sense.

So, can we add units to our `double`s? Can we tell the compiler
that `Person` requires a quantity in kg and one in meter, in that order?

It is quite easy to do so, in many languages: we can simply define a new type.

```c++
class Weight
{
    double magnitude;

public:
    Weight(double magnitude) : magnitude(magnitude) { }
};


class Height
{
    double magnitude;

public:
    Height(double magnitude) : magnitude(magnitude) { }
};


class Person
{
public:
    Person(Weight weight, Height height)
        : weight(weight), height(height) { }

    Weight weight;
    Height height;
};
```

By introducing distinct types, the compiler can enforce stricter rules:
now we cannot make the mistake of accidentally passing
weight and height in the wrong order.

## Operations

What about interacting with `Weight` and `Height`? Currently, objects
of these types have no operations defined on them. They're essentially useless.

Well then, let's start defining the necessary operations on them. Let's consider
weight: in essence, it's a numeric value, so we should define the following:

* Equality `==` and inequality `!=`.
* Comparison `<`, `>`, `<=` and `>=`.
* A `<<` on output streams would be nice.
* Arithmetic operators such as `+`, `-`, `*` and `/`.

This sounds like a lot of work. And it sure is. Plus, we need to do it twice:
once for `Weight`, once for `Height`. And what if we add more `Person` properties, such
as `Age age`? It's starting to look as if this was not really a good idea.

If classes have functionality in common, the general rule is to
move this functionality into a separate class and have other classes extend it:

```c++
class Common
{
    double magnitude;

public:
    bool operator ==(const Common& other) const;
    bool operator !=(const Common& other) const;
    bool operator <(const Common& other) const;
    bool operator <=(const Common& other) const;
    bool operator >=(const Common& other) const;
    bool operator >(const Common& other) const;

    Common& operator +(const Common& other) const;
    Common& operator -(const Common& other) const;
    Common& operator *(const Common& other) const;
    Common& operator /(const Common& other) const;
};
```

Unfortunately, this approach suffers from a number of serious shortcomings,
the worst of which is that it breaks down the walls between our types:

```c++
Weight w(10);
Height h(5);

auto x = w + h;
```

The addition is allowed: `w` and `h` get upcasted to `Common` and the sum
produces a `Common` object, i.e., `x` has type `Common`. This is exactly
what we're trying to prevent from happening.

Private inheritance would prevent the troublesome upcasting,
but then we would also hide all operations from the outside world.

What now?

## Templates To The Rescue

Consider the following code:

```c++
template<typename T>
class NewDouble
{
    double value;

public:
    NewDouble(double value) : value(value) { }

    bool operator ==(const NewDouble<T>& other) const;
    bool operator !=(const NewDouble<T>& other) const;
    // operators
};
```

This definition is a bit weird: we introduce a type parameter `T`
yet do not refer to it anywhere. It looks as if someone made a mistake
and meant to have `value` to be a `T` instead of a `double`.

The `T` does serve a purpose: it can be used to create
distinct kind of NewDoublees. For example:

```c++
NewDouble<double> x(5);
NewDouble<int> y(6);
auto z = x + y; // Does not compile
```

Even though `NewDouble<double>` and `NewDouble<int>` are essentially the same kind of
object, their types are different. As far as the compiler is concerned,
they are nothing alike and will be treated as incompatible values.
We can now add two `NewDouble<double>`s together, or two `NewDouble<int>`s,
or two `NewDouble<Whatever>`s, but we cannot mix.

We can now use `NewDouble<double>` for weights and `NewDouble<int>` for heights.
The `T` types can be chosen completely arbitrarily. `NewDouble<bool>` and `NewDouble<std::string>` will work just as well.
But admittedly, it's quite brittle: we need to make sure
that for every "new" `double` type (weight, height, age, ...)
we make use of a different `T`:

```c++
class Person
{
    NewDouble<int> weight;
    NewDouble<double> height;
    NewDouble<bool> age;
    NewDouble<int> iq; // Oh no! iq and weight have same T. Bug!
};
```

Apart from the brittleness, this also looks incredibly confusing:
those `int`, `double`, `bool` types are terribly misleading.
We do want robust type-checked code, but we also want it
to be readable. Now it just looks as if we made our
code intentionally obscure so as to justify our fancy
typing constructs. As James May would put it, it's an ingenious
solution to a problem which should never have existed in the first place.

## Making Things Sane Again

We can reassure you: there is light at the end of the tunnel and we will
soon reach it.

Let's first start by introducing sensible names for our
new types. Now we need to remember that `NewDouble<int>`
should be used for weights and `NewDouble<double>` for heights,
or was it the other way around? If we make a mistake here,
all our work has been for naught.

We can introduce names easily by subclassing:

```c++
struct Weight : NewDouble<int>    { };
struct Height : NewDouble<double> { };
struct Age    : NewDouble<bool>   { };
```

Much better. Now, let's deal with these `T`s. Is there a way
to avoid the bookkeeping necessary to prevent us from accidentally reusing the same `T`?

The `T` can be *any* type, as long as it's different from other types.
It just so happens that at the very same moment, we are
defining a *new* type, namely `Weight`, or `Height`, or `Age`.
Why not simply use that type?

```c++
struct Weight : NewDouble<Weight> { };
struct Height : NewDouble<Height> { };
struct Age    : NewDouble<Age>    { };
```

This is perfectly acceptable code, as strange as it may seem.
We're defining a new type `Weight` and at the same
time using it as tag to differentiate it from other `NewDouble` types.

## Generalizing

Right now, this works only for `double`s. There's no reason
for this design to be that rigid. We generalize it by adding a type parameter:

```c++
template<typename T, typename TAG>
class tagged
{
private:
    T _value;

public:
    tagged() : _value() { }

    explicit tagged(const T& value) : _value(value) { }
};
```

Here, `T` is the 'actual' type of the data and `TAG` is
the dummy type parameter used to force a difference in typing.
For example,

```c++
struct Weight       : tagged<double, Weight>            { };
struct Height       : tagged<double, Height>            { };
struct Name         : tagged<std::string, Name>         { };
struct EmailAddress : tagged<std::string, EmailAddress> { };
```

In order to get access to `_value`, we introduce a function `value`:

```c++
template<typename T, typename TAG>
class tagged
{
private:
    T _value;

public:
    tagged() : _value() { }

    explicit tagged(const T& value) : _value(value) { }

    // Gives read/write access to _value
    friend T& value(tagged<T, TAG>& st)
    {
        return st._value;
    }

    // Gives read access to _value
    friend const T& value(const tagged<T, TAG>& st)
    {
        return st._value;
    }
};
```

This function allows you to "break open" your `Weight`/`Height`/... object.
Ideally, you'll never need this, but it's always good to have a safety hatch.
Allowing this does not defeat the purpose of `tagged`: the breaking open
needs to be done explicitly and hence cannot happen by accident.
This is typical in the C++ world: we're protecting against Murphy,
not against Machiavelli. In other words, we want to eliminate your blunders
but not your freedom.

Also, we'd like to make `tagged`'s constructor available in the subclasses.
Remember that in C++, Java and C# constructors are not inherited and
need to be redefined at every level.

```c++
struct Weight       : tagged<double, Weight>            { using tagged::tagged; };
struct Height       : tagged<double, Height>            { using tagged::tagged; };
struct Name         : tagged<std::string, Name>         { using tagged::tagged; };
struct EmailAddress : tagged<std::string, EmailAddress> { using tagged::tagged; };
```

`using tagged::tagged` is a C++ trick to express your wish to
copy all of `tagged`'s constructors.

## Operators

Right now, `tagged` does not yet define any operators (`==`, `+`, ...).
In case we need to compare heights, we need to break them open:

```c++
Weight w1(80), w2(100);

if ( value(w1) < value(w2) )
{
    ...
}
```

Requiring this is a terrible idea: it simply makes your code less readable
and it circumvents our entire typing construct:

```c++k
Weight w(80);
Height h(1.8);

if ( value(w) < value(h) ) // Bug! Yet compiler does not complain
{
    ...
}
```

We want our code to look as shown below:

```c++
Weight w1(80), w2(100);
Height h1(1.7), h2(1.8);

if ( w1 < w2 ) // Ok
    ...

if ( h1 < h2 ) // Ok
    ...

if ( w1 < h2 ) // Mixing of types. Compiler should reject it
```

To achieve this, we can simply add the necessary definitions to `tagged`, but this might be too permissive.
There might be operators we *don't* want defined on our types. Is there perhaps an easy way to make a selection of
which operators should be available?

## Traits and Mixins

Oftentimes, functionality comes in groups. For example, if you define `==`, you generally also define `!=`.
Similarly, `<`, `>`, `<=` and `>=` are (almost) always defined together.

Additionally, in order to avoid redundancy, they are defined in terms of each other. For example,
`!=` is often defined in terms of `==`:

```c++
bool operator !=(const T& t) const
{
    return !(*this == t);
}
```

All classes sporting a `!=` operator will have this exact definition in it. Likewise for the relational
operators `<`, `>`, etc.: they are all defined in terms of one central operation.

Some language offer the concept of *traits* and/or *mixins* (there are some differences between them,
but the core idea is the same) are auxiliary classes that provide related function definitions that can
be 'included' in a class. C++ allows us to fake this using inheritance.
An example would be (the code has been slightly simplified for the sake of clarity):

```c++
template<typename T>
struct equality
{
    bool operator ==(const T& other) const
    {
        return equal_to(other);
    }

    bool operator !=(const T& other) const
    {
        return !equal_to(other);
    }
};

struct SomeType : equality<SomeType>
{
    bool equal_to(const SomeType& other) const { ... }
};
```

Here, we define one 'central method' `equal_to`. By inheriting from
`equality<SomeType>`, we import definitions for `==` and `!=` into `SomeTime`.
Both internally rely on `equal_to` to compute their results.

We can now define specialized traits/mixins for adding operators to `tagged` as follows:

```c++
template<typename T>
struct equality
{
    friend bool operator ==(const T& x, const T& y) { return value(x) == value(y); }
    friend bool operator !=(const T& x, const T& y) { return value(x) != value(y); }
};

template<typename T>
struct ordered : equality<T>
{
    friend bool operator < (const T& x, const T& y) { return value(x) <  value(y); }
    friend bool operator <=(const T& x, const T& y) { return value(x) <= value(y); }
    friend bool operator > (const T& x, const T& y) { return value(x) >  value(y); }
    friend bool operator >=(const T& x, const T& y) { return value(x) >= value(y); }
};
```

Note how `ordered` is a subtype of `equality`: once you have the relational operators `<=` and `>=`, etc.
it makes little sense to not also have `==` and `!=`. So, if you decide to include `ordered`,
you automatically also get `equality`. In other words, including `ordered` in your class
will define the operators `==`, `!=`, `<`, `>`, `<=` and `>=`.

Let's apply this on our `Height` type. It makes sense of a `Height` to be compared
to another `Height`, so we want `==` and `!=`. It also makes sense to say
one `Height` is greater or smaller than another, meaning we'd also like `<`,
`>`, `<=` and `>=` to be defined. This can be achieved as follows:

```c++
struct Height : tagged<double, Height>, ordered<Height> { using tagged:tagged; };
```

This single line of code gives us the following:

* We have a separate type for heights thanks to `tagged`. We don't need to worry about accidentally confusing it with weights as this would be caught by the compiler.
* `ordered` adds definitions for `==`, `!=`, `<`, `>`, `<=` and `>=`.
* `using tagged::tagged` allows you to construct a `Height` object, e.g., `Height(1.80)`.
* You can force the `Height` abstraction open using `value`, e.g., `value(height)++`.

## Performance

Those worried that this might negatively impact performance, rest assured:
C++ compilers can see that a `Height` is in reality nothing more than
a `double` and will compile it as such. All typing information exists only at compile time.

C++ offers you the ability to introduce new abstractions without incurring a performance penalty.
Such abstractions are called *zero-cost abstractions*. These let you write better
code (more readable, structured, typed, ...) without having to make sacrifices.