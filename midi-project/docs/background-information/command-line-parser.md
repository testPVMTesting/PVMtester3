# Command Line Parser

## Example Usage

A quick example of how to use the `CommandLineParser` class:

```c++
int main(int argc, char** argv)
{
    std::string stringValue = "default";
    unsigned uintValue = 5;
    bool boolValue = false;

    shell::CommandLineParser parser;

    parser.add_argument("-s", &stringValue);
    parser.add_argument("-i", &uintValue);
    parser.add_argument("-b", &boolValue);
    parser.process(argc, argv);

    std::cout << "stringValue=" << stringValue << std::endl;
    std::cout << "uintValue=" << uintValue << std::endl;
    std::cout << "boolValue=" << (boolValue ? "true" : "false") << std::endl;

    std::cout << "Positional arguments:" << std::endl;
    std::cout << "---" << std::endl;
    for (auto positional : parser.positional_arguments())
    {
        std::cout << positional << std::endl;
    }
    std::cout << "---" << std::endl;
}
```

```bash
# All parameters are optional
$ ./midi.exe
stringValue=default
uintValue=5
boolValue=false
Positional arguments:
---
---

$ ./midi.exe -s bla
stringValue=bla
uintValue=5
boolValue=false
Positional arguments:
---
---

# Note that b does not have a parameter since it corresponds to a bool
# -b absent -> false
# -b present -> true
$ ./midi.exe -s bla -i 1 -b
stringValue=bla
uintValue=1
boolValue=true
Positional arguments:
---
---

# Positional parameters are those that have no preceding -tag
# or in other words, the leftover arguments
$ ./midi.exe ABC -s bla XYZ -i 1 -b 123
stringValue=bla
uintValue=1
boolValue=true
Positional arguments:
---
ABC
XYZ
123
---
```