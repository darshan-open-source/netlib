//bug free
#pragma once

#include<string>
#include<vector>
#include<map>
#include"stringutil.h"

class getParser {
public:
    getParser(std::string);
    std::multimap<std::string, std::string> get() { return  mains; };
private:
    std::string payload;
    void process();
    std::multimap<std::string, std::string> mains;
};
getParser::getParser(std::string fullpath) {
    int f1 = fullpath.find('?', 0);
    if (f1 != std::string::npos)
    {

        for (int i = f1 + 1; i < fullpath.length(); i++) {

            payload.append(1, fullpath.at(i));
        }
    }
    process();
    payload.clear();
}
void getParser::process()
{

    std::vector<std::string> temp = split(payload, '&');
    for (int i = 0; i < temp.size(); i++) {
        std::vector<std::string> temp2 = split(temp.at(i), '=');
        if (temp2.size() == 2) {
            mains.insert(std::pair<std::string, std::string>(stripString(temp2.at(0)), stripString(temp2.at(1))));
        }
    }
    temp.clear();
}

