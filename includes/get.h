// bug free
#pragma once

#include <string>
#include <vector>
#include <map>
#include <stringutil.h>

class getParser
{
public:
    getParser(std::string);
    std::multimap<std::string, std::string> get() { return mains; };

private:
    std::string payload;
    void process();
    std::multimap<std::string, std::string> mains;
};
