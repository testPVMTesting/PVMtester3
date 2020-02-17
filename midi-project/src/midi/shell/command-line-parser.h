#pragma once

#include <functional>
#include <string>
#include <vector>
#include <list>
#include <map>

namespace shell
{
    class CommandLineParser
    {
    public:
        void add_argument(const std::string& prefix, std::function<void()> processor);
        void add_argument(const std::string& prefix, std::function<void(const std::string&)> processor);

        void add_argument(const std::string& prefix, bool*);
        void add_argument(const std::string& prefix, unsigned*);
        void add_argument(const std::string& prefix, std::string*);

        void process(const std::vector<std::string>&);
        void process(int argc, char** argv);
        std::vector<std::string> positional_arguments() const;

    private:
        void add_argument(const std::string& prefix, std::function<void(std::list<std::string>&)>);
        void process(std::list<std::string>&);
        bool is_prefix_in_use(const std::string& prefix) const;

        std::vector<std::string> m_positional_arguments;
        std::map<std::string, std::function<void(std::list<std::string>&)>> m_map;
    };
}