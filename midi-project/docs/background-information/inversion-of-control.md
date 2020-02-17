# Inversion of Control

Say you have some sort of data source from which you receive data. There are multiple kinds of data,
each of which might need to be dealt with differently. A few examples:

* A GUI which produces events such as key presses and mouse movements.
* A webserver which receives GET, POST, PUT, ... requests.
* A file system contains files and directories.
* A MIDI file containing different kinds of events: note on, note off, ...

Take for example a GUI: at the core of a GUI system (this is true for both Windows and Linux --- MacOS probably
too, I never bothered to check) resides a *message queue* in which the OS dumps notifications
regarding user input and OS events.
In [Windows](https://wiki.winehq.org/List_Of_Windows_Messages) terms,
whenever the user presses down a key, a `WM_KEYDOWN` is added to the queue. Other examples are

* `WM_MOUSEMOVE`: the user has moved the mouse.
* `WM_SIZE`: the user has resized the window.
* `WM_CLOSE`: the user has closed the window.
* And hundreds of others.

Each event is accompanied by extra data. For example, `WM_KEYDOWN` carries information with it
about which key has been pressed.

Dealing with these messages is done using a [long if-then-else structure](https://docs.microsoft.com/en-us/windows/desktop/learnwin32/writing-the-window-procedure):

```cpp
while ( program_is_running )
{
    message = get_next_message();

    switch ( message.type )
    {
        case WM_KEYDOWN:
            char key = message.key;
            // process key

        case WM_MOUSEMOVE:
            int x = message.x;
            int y = message.y;
            // process mouse move

        case WM_SIZE:
            int new_width = message.width;
            int new_height = message.height;
            // process resize

        case WM_CLOSE:
            // process close

        // process other events
    }
}
```

Let's simplify this to a very specific case to keep the examples manageable:
instead of having a GUI with hundreds of different messages, we have
a text file containing nothing but one of two characters: `A` or `B`.

Our data processing code becomes:

```cpp
while ( file_not_ended() )
{
    char c = read_next_char();

    switch ( c )
    {
        case 'A':
            // Deal with A

        case 'B':
            // Deal with B
    }
}
```

Let's consider some algorithms. Say we want to count the number of `A`s and `B`s:

```cpp
unsigned a = 0, b = 0;

while ( file_not_ended() )
{
    char c = read_next_char();

    switch ( c )
    {
        case 'A':
            a++;
            break;

        case 'B':
            b++;
            break;
    }
}
```

The code below swaps `A`s and `B`s around:

```cpp
while ( file_not_ended() )
{
    char c = read_next_char();

    switch ( c )
    {
        case 'A':
            std::cout << 'B';
            break;

        case 'B':
            std::cout << 'A';
            break;
    }
}
```

This algorithm checks if the file contains alternating `A`s and `B`s:

```cpp
bool correct = true;
char last = 'B';

while ( file_not_ended() )
{
    char c = read_next_char();

    switch ( c )
    {
        case 'A':
            if ( last != 'B' ) correct = false;
            last = 'A';
            break;

        case 'B':
            if ( last != 'A' ) correct = false;
            last = 'B';
            break;
    }
}
```

As you can see, the examples above all share the same structure. In this kind of situation,
it is best to factor out the common code. It might seem a bit overkill for this case
since the common code is not particularly complex. However, it would certainly be warranted
if the input file were more complex. For example, `A`s and `B`s could be accompanied by extra
data which would need to be parsed. This code would be repeated for all three algorithms above.

So, anyway, let's factor out the common code. Our approach is as follows:

```cpp
class Receiver
{
public:
    virtual void process_a() = 0;
    virtual void process_b() = 0;
};

void read_file(Receiver& receiver)
{
    while ( file_not_ended() )
    {
        char c = read_next_char();

        switch ( c )
        {
            case 'A':
                receiver.process_a();
                break;

            case 'B':
                receiver.process_b();
                break;
        }
    }
}
```

As you can see, we have declared an "interface" `Receiver` with two methods:
`process_a` and `process_b`. `read_file` accepts a `Receiver` object.
Whenever it encounters an `A` in the file, it calls `process_a`, and likewise for `B`.

The three algorithms above can then be rewritten as follows:

```cpp
class Counter : public Receiver
{
    int a, b;

public:
    Counter() : a(a), b(b) { }

    void process_a() override { ++a; }
    void process_b() override { ++b; }
};
```

```cpp
class Swapper : public Receiver
{
public:
    void process_a() override { std::cout << "B"; }
    void process_b() override { std::cout << "A"; }
};
```

```cpp
class AlternateCheck : public Receiver
{
    bool result;
    char last;

public:
    Counter() : result(true), last('B') { }

    void process_a() override
    {
        if ( last != 'B' ) result = false;
        last = 'A';
    }

    void process_b() override
    {
        if ( last != 'A' ) result = false;
        last = 'B';
    }
};
```

This approach should not be unfamiliar to you. GUI libraries use the same technique:
you can specify `on_click`, `on_key_down`, ... methods, which will be called internally
by the library whenever the corresponding event occurs.

This technique, known as inversion of control, has multiple advantages:

* It reduces code duplication.
* The `Receiver` subclasses contain only code related to processing the different cases, which improves readability.
* `Receiver` objects can be kept small and be combined so as to achieve more complex functionality.
* Whenever new types of events are introduced, an extra method can be added to `Receiver`. The compiler will then tell you where all subclasses of `Receiver` need to be updated. This kind of compiler assistance would be lacking otherwise.
