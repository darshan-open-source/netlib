#include<cookies.h>
cookieParser::cookieParser(std::string cookiePayload)
{
    if (cookiePayload.empty())return;
    
    string cookie = stripString(cookiePayload);
    std::vector<string> cookies = split(cookie, ';');

    for (size_t i = 0; i < cookies.size(); i++)
    {
        std::vector<std::string> text = split(stripString(cookies.at(i)), '=');

        if (!text.at(0).empty())
        {
            COOKIE c;
            c.name = text.at(0);
            if (text.size() == 2)
            {
                if (!text.at(1).empty())
                {
                    c.value = text.at(1);
                }
            }
            this->cookies.push_back(c);
        }
    }
}
