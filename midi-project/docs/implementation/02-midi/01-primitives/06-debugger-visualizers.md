# Debugger Visualizers

This part is optional, since this step is Visual Studio specific, which not everyone uses.

To aid in your debugging endeavors, it might be a good idea to define some debugger
visualizers for the `Channel`, `NoteNumber`, `Instrument`, `Time` and `Duration` types.

We'll give you one for free:

```xml
  <Type Name="midi::Time">
    <DisplayString>{{time = {_value,d}}}</DisplayString>
  </Type>
```
