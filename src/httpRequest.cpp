#include <httpRequest.h>

MULTIPART_DATA httpRequest::_GET_MULTIPART(std::string name)
{
    MULTIPART_DATA md;
    if (h.payloadType != PAYLOAD_TYPE_MULTIPART_PAYLOAD)
    {
        return md;
    }
    std::vector<MULTIPART_DATA> multipart_data = std::get<std::vector<MULTIPART_DATA>>(h.payload);
    for (int i = 0; i < multipart_data.size(); i++)
    {
        if (multipart_data.at(i).name.compare(name) == 0)
        {
            return multipart_data.at(i);
        }
    }
    return md;
}

httpRequest::httpRequest(HTTP_HEADER header, std::vector<URL_DATA> udata) : urldatas(udata)
{
    h = header;
    cookieParser p(h.cookieString);
    cookies = p.cookies;
}

std::vector<COOKIE> httpRequest::getAllCookies()
{
    return cookies;
}

std::string httpRequest::getCookie(std::string name)
{

    for (int i = 0; i < cookies.size(); i++)
    {
        if (name.compare(cookies.at(i).name) == 0)
        {
            return cookies[i].value;
        }
    }
    std::string tc;

    return tc;
}

std::string httpRequest::_GET(std::string name)
{
    std::string s;
    if (h.payloadType != PAYLOAD_TYPE_GET_PAYLOAD)
    {
        return s;
    }
    std::multimap<std::string, std::string> payload = std::get<std::multimap<std::string, std::string>>(h.payload);
    std::multimap<std::string, std::string>::iterator it;
    for (it = payload.begin(); it != payload.end(); it++)
    {
        if (it->first.compare(name) == 0)
        {
            return it->second;
        }
    }
    return s;
}
std::string httpRequest::_POST(std::string name)
{
    std::string s;
    if (h.payloadType != PAYLOAD_TYPE_POST_PAYLOAD)
    {
        return s;
    }

    std::multimap<std::string, std::string> payload = std::get<std::multimap<std::string, std::string>>(h.payload);

    std::multimap<std::string, std::string>::iterator it;
    for (it = payload.begin(); it != payload.end(); it++)
    {
        if (it->first.compare(name) == 0)
        {
            return it->second;
        }
    }
    return s;
}
std::string httpRequest::_REQUEST(std::string name)
{
    std::string s;
    if (!(h.payloadType == PAYLOAD_TYPE_GET_PAYLOAD || h.payloadType == PAYLOAD_TYPE_POST_PAYLOAD))
    {
        return s;
    }
    std::multimap<std::string, std::string> payload = std::get<std::multimap<std::string, std::string>>(h.payload);
    std::multimap<std::string, std::string>::iterator it;
    for (it = payload.begin(); it != payload.end(); it++)
    {
        if (it->first.compare(name) == 0)
        {
            return it->second;
        }
    }
    return s;
}
HTTP_HEADER httpRequest::getFullHeader()
{
    return h;
}

bool httpRequest::isSessionSet()
{

    string cookieval = getCookie(SESSION_ID);

    if (!cookieval.empty() && cookieval.length() == SESSION_VALUE_SIZE)
    {

        string s = SESSION_ID;
        s += "_";
        s += cookieval;
        s += ".json";
        std::filesystem::path p(session_directory);
        p.replace_filename(std::filesystem::path(s));
        if (std::filesystem::exists(p))
        {

            sessionFile = p.generic_string();
            return true;
        }
    }
    return false;
}

sessions httpRequest::getSession()
{
    if (isSessionSet())
    {
        return sessions(sessionFile);
    }
    return sessions();
}

std::string httpRequest::_GetUrlParam(std::string name)
{

    for (int i = 0; i < urldatas.size(); i++)
    {
        if (urldatas.at(i).name.compare(name) == 0)
        {
            return urldatas.at(i).value;
        }
    }
    string s;
    return s;
}
std::string httpRequest::getOtherPayload()
{
    if(h.payloadType == PAYLOAD_TYPE_OTHER){


        return std::get<std::string>(h.payload);
    }
    std::string s;
    return s;
};
