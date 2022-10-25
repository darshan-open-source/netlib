#pragma once
#include <string>
#include "enums.h"
#include "stringutil.h"
#include "standalone.h"
#include "response_header.h"
class cookieParser
{
public:
    cookieParser(std::string);
    COOKIE getCookie() { return c; };

private:
    COOKIE c;
};
cookieParser::cookieParser(std::string payload)
{

    string cpayload = stripString(payload.substr(payload.find(':') + 1, payload.length() - 1));
    string sp = stripString(payload.substr(0, payload.find(":")));
    if (toUpper(sp).compare("SET-COOKIE") == 0)
    {

        std::vector<std::string> vals = split(cpayload, ';');
        std::vector<std::string> inner_val2 = split(vals.at(0), '=');
        c.name = stripString(inner_val2.at(0));
        c.value = stripString(inner_val2.at(1));

        for (size_t i = 1; i < vals.size(); i++)
        {
            std::vector<std::string> inner_val = split(vals.at(i), '=');
            if (inner_val.size() == 2)
            {
                if (toUpper(inner_val.at(0)).compare("DOMAIN") == 0)
                {
                    c.domain = stripString(inner_val.at(1));
                }
                else if (toUpper(inner_val.at(0)).compare("PATH") == 0)
                {
                    c.path = stripString(inner_val.at(1));
                }
                else if (toUpper(inner_val.at(0)).compare("MAXAGE") == 0)
                {
                    c.maxAge = atoi(stripString(inner_val.at(1)).data());
                }
                else if (toUpper(inner_val.at(0)).compare("EXPIRY") == 0)
                {
                    c.expiry = stripString(inner_val.at(1));
                }
                else if (toUpper(inner_val.at(0)).compare("SAMESITE") == 0)
                {
                    string sxw = toUpper(stripString(inner_val.at(1)));
                    if (sxw.compare("NONE") == 0)
                    {
                        c.sameSite = SAMESITE_TYPE_NONE;
                    }
                    else if (sxw.compare("LAX") == 0)
                    {
                        c.sameSite = SAMESITE_TYPE_LAX;
                    }
                    else if (sxw.compare("STRICT") == 0)
                    {
                        c.sameSite = SAMESITE_TYPE_STRICT;
                    }
                    else
                    {
                        c.sameSite = SAMESITE_TYPE_UNDEFINED;
                    }
                }
            }
            else if (inner_val.size() == 1)
            {
                if (toUpper(inner_val.at(0)).compare("SECURE") == 0)
                {
                    c.secure = true;
                }
                else if (toUpper(inner_val.at(0)).compare("HTTPONLY") == 0)
                {
                    c.httpOnly = true;
                }
            }
        }
    }
};
class responseParser
{
public:
    responseParser(std::string);
    HTTP_RESPONSE header;

private:
    std::string s;
};
responseParser::responseParser(std::string data)
{
    int j = data.find("\r\n\r\n");
    s = data.substr(0, j + 2);
    std::vector<std::string> x = split(s, '\r');
    std::vector<std::string> a1 = split(x.at(0), ' ');
    if (toUpper(stripString(a1.at(0))).compare("HTTP/1.1") == 0)
    {
        header.version = HTTP_VERSION_HTTP1;
    }

    else if (toUpper(stripString(a1.at(0))).compare("HTTP/2") == 0)
        header.version = HTTP_VERSION_HTTP2;
    else
        header.version = HTTP_VERSION_UNDEFINED;

    header.status_code = (HTTP_STATUS_CODE)atoi(a1.at(1).data());

    x.erase(x.begin());

    for (size_t i = 0; i < x.size(); i++)
    {
        x.at(i).erase(0, 1);
        int xmp = x.at(i).find(':');
        if (xmp == std::string::npos)
            continue;
        string key = x.at(i).substr(0, xmp);
        string value = x.at(i).substr(xmp + 1, x.at(i).length() - xmp);
        if (toUpper(stripString(key)).compare("SERVER") == 0)
        {
            header.host = stripString(value);
        }
        else if (toUpper(stripString(key)).compare("CONNECTION") == 0)
        {
            if (toUpper(stripString(value)).compare("KEEP-ALIVE") == 0)
                header.connection_type = CONNECTION_TYPE_KEEP_ALIVE;
            else if (toUpper(stripString(value)).compare("CLOSE") == 0)
                header.connection_type = CONNECTION_TYPE::CONNECTION_TYPE_CLOSE;
            else
                header.connection_type = CONNECTION_TYPE::CONNECTION_TYPE_UNDEFINED;
        }
       
        else if (toUpper(stripString(key)).compare("CONTENT-TYPE") == 0)
        {

            header.content_type = stripString(value);
        }
        
        else if (toUpper(stripString(key)).compare("SET-COOKIE") == 0)
        {
            cookieParser cp(value);
            header.cookies.push_back(cp.getCookie());
        }
      
        else if (toUpper(stripString(key)).compare("CONTENT-LENGTH") == 0)
        {
            header.contentLength = stoll(stripString(value));
        }
        
        else
        {

            header.others.insert(std::make_pair<std::string, string>(stripString(key), stripString(value)));
        }
    }
}

class responseBuilder:public responseHeaderBuilder
{
public:
    responseBuilder(HTTP_RESPONSE res);
    std::string getResponse() { return header; };

private:
    std::string header;
};
responseBuilder::responseBuilder(HTTP_RESPONSE res):responseHeaderBuilder(res.version,res.status_code)
{
    if (!res.host.empty())
    {
        set("Server", res.host);
    }
    if (!res.cookies.empty())
    {
        for (size_t i = 0; i < res.cookies.size(); i++)
        {
            setCookie(res.cookies.at(i));
        }
    }
    if (res.connection_type != CONNECTION_TYPE_UNDEFINED)
    {
        setConnectionType(res.connection_type);
    }
    if (res.contentLength != 0)
    {
        setContentLength(res.contentLength);
    }
    if (!res.location.empty())
    {
        set("Location", res.location);
    }
    if(res.others.size() !=0){
        for (std::multimap<std::string,std::string>::iterator i = res.others.begin(); i != res.others.end(); i++)
        {
                   set(i->first,i->second);
        }
        
    }
    buildHeader();


    header = getHeader();



}