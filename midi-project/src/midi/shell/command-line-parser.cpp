#include "command-line-parser.h"
#include "logging.h"
#include <assert.h>


using namespace shell;

void CommandLineParser::add_argument(const std::string& prefix, std::function<void()> processor)
{
    std::function<void(std::list<std::string>&)> wrapper = [processor](std::list<std::string>& arguments) -> void {
        processor();
    };

    add_argument(prefix, wrapper);
}

void CommandLineParser::add_argument(const std::string& prefix, std::function<void(const std::string&)> processor)
{
    std::function<void(std::list<std::string>&)> wrapper = [prefix, processor](std::list<std::string>& arguments) -> void {
        CHECK(!arguments.empty())<< "Command line argument " << prefix << " expects an argument";
        
        auto head = arguments.front();
        arguments.pop_front();

        processor(head);
    };

    add_argument(prefix, wrapper);
}

void CommandLineParser::add_argument(const std::string& prefix, bool* target)
{
    add_argument(prefix, [target]() {
        *target = true;
    });
}

void CommandLineParser::add_argument(const std::string& prefix, std::string* target)
{
    add_argument(prefix, [target](const std::string& argument) {
        *target = argument;
    });
}

void CommandLineParser::add_argument(const std::string& prefix, unsigned* target)
{
    add_argument(prefix, [target](const std::string& argument) {
        *target = std::stoi(argument);
    });
}

void CommandLineParser::add_argument(const std::string& prefix, std::function<void(std::list<std::string>&)> processor)
{
    CHECK(!is_prefix_in_use(prefix)) << "Clashing prefixes";

    m_map[prefix] = processor;
}

void CommandLineParser::process(const std::vector<std::string>& args)
{
    std::list<std::string> arguments(args.begin(), args.end());
    process(arguments);
}

void CommandLineParser::process(std::list<std::string>& arguments)
{
    while (!arguments.empty())
    {
        auto head = arguments.front();
        arguments.pop_front();

        if (head[0] == '-')
        {
            auto it = m_map.find(head);

            CHECK(it != m_map.end()) << "Unknown command " << head;

            auto processor = *it;
            processor.second(arguments);
        }
        else
        {
            m_positional_arguments.push_back(head);
        }
    }
}

void CommandLineParser::process(int argc, char** argv)
{
    std::vector<std::string> vec(argv + 1, argv + argc);
    process(vec);
}

bool CommandLineParser::is_prefix_in_use(const std::string& prefix) const
{
    return m_map.find(prefix) != m_map.end();
}

std::vector<std::string> CommandLineParser::positional_arguments() const
{
    return m_positional_arguments;
}