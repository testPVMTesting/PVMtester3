# Debugger Visualizers

While debugging you can view your variable's value.
Visual Studio will try its best to show
you relevant information, but there
are times where being defining our
own visualization rules would improve
readability. This is where [debugger visualizers](https://docs.microsoft.com/en-us/visualstudio/debugger/create-custom-views-of-native-objects?view=vs-2017#BKMK_Expressions_and_formatting) come in.

For example, say we define a new type age (see [strong typedefs](./strong-typedefs.md)):

```cpp
struct age : tagged<int, age>, ordered<age> { using tagged::tagged; };

age a(5);
```

When querying the value of `a` while debugging, Visual Studio
shows you `{ ... }`. Hardly impressive. There is the
possibility to expand the view, which results in

```text
a                      { ... }
  tagged<int, age>     { _value = 5 '\x5' }
  ordered<age>         { ... }
```

We'd prefer it if we didn't have to press the expand arrow
to find out what `a`'s value is.

To define a debugger visualizer for values of type `age`,
start by adding a `.natvis` file to your project:

* Right click on the project
* Add
* New Item
* Under Utility, choose Debugger visualization file

Open this file. It should contain

```xml
<?xml version="1.0" encoding="utf-8"?>
<AutoVisualizer xmlns="http://schemas.microsoft.com/vstudio/debugger/natvis/2010">
</AutoVisualizer>
```

Add the following rule for the type `age`:

```diff
<?xml version="1.0" encoding="utf-8"?>
<AutoVisualizer xmlns="http://schemas.microsoft.com/vstudio/debugger/natvis/2010">
+   <Type Name="age">
+     <DisplayString>{{age = {_value,d}}}</DisplayString>
+   </Type>
</AutoVisualizer>
```

The `DisplayString` determines what is shown while debugging.
With the configuration above, viewing an `age` variable will reveal

```text
a        { age = 5 '\x5' }
```

which is more helpful that the old `{ ... }`.