
// use this for only debugging purpose avoid use in production build

#pragma once
#include <iostream>
#include <map>
class console
{
public:
    static void log(std::string s);
    static void log(int);
    template <typename t>
    static void log(std::vector<t> data);
    template <typename u, typename v>
    static void log(std::multimap<u, v> data);
};
void console::log(std::string s)
{
    std::cout
        << s << std::endl;
}
void console::log(int x)
{
    std::cout
        << x << std::endl;
}
template <typename t>
void console::log(std::vector<t> data)
{
    for (typename std::vector<t>::iterator i = data.begin(); i != data.end(); i++)
    {
        std::cout
            << *i << std::endl;
    }
}
template <typename u, typename v>
static void log(std::multimap<u, v> data)
{

    for (typename std::multimap<u, v>::iterator i = data.begin(); i != data.end(); i++)
    {

        std::cout << (*i).first << "   " << (*i).second << std::endl;
    }
}