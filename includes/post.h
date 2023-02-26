//bug free


#pragma once

#include <string>
#include<map>
#include<vector>
#include<stringutil.h>

class postParser {
public:
    postParser(std::string);
    std::multimap<std::string, std::string> get() { return  mains; };
private:
    std::string payload;
    void process();
    std::multimap<std::string, std::string> mains;


};
