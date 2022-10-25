//bug free


#pragma once

#include <string>
#include<map>
#include<vector>
#include"stringutil.h"

class postParser {
public:
    postParser(std::string);
    std::multimap<std::string, std::string> get() { return  mains; };
private:
    std::string payload;
    void process();
    std::multimap<std::string, std::string> mains;


};
postParser::postParser(std::string payload) {
    this->payload = payload;
    process();

    
}
void postParser::process()
{
    std::vector<std::string> temp = split(payload, '&');
    for (int i = 0; i < temp.size(); i++) {
        std::vector<std::string> temp2 = split(temp.at(i), '=');
        if (temp2.size() == 2) {

            
            mains.insert( std::make_pair(temp2.at(0), temp2.at(1)));
            

        }
    }

}
