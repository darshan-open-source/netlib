#include<post.h>

postParser::postParser(std::string payload) {
    //perform no validation on input

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
