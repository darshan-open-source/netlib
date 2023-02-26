#include<get.h>
getParser::getParser(std::string fullpath)
{
    int f1 = fullpath.find('?', 0);
    if (f1 != std::string::npos)
    {
        payload += fullpath.substr(f1 + 1, fullpath.length() - (f1 + 1));
        process();
        payload.clear();
    }
}
void getParser::process()
{

    std::vector<std::string> temp = split(payload, '&');
    for (int i = 0; i < temp.size(); i++)
    {
        std::vector<std::string> temp2 = split(temp.at(i), '=');

        if (temp2.size() == 2)
        {

                mains.insert(std::pair<std::string, std::string>(stripString(temp2.at(0)), temp2.at(1)));
        }
     
    }
    temp.clear();
}
