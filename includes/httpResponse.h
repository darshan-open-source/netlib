//bug free
#pragma once

#include <memory>
#include "SocketServer.h"
#include "enums.h"
#include "response_header.h"
#include "sessions.h"
#include <filesystem>
#include <inja/inja.hpp>

class httpResponse : public responseHeaderBuilder
{
public:
    httpResponse(std::shared_ptr<darshan::network::SocketConnection> s, HTTP_HEADER x) : writer(s) { h = x; };
    httpResponse(){} ;
    RESPONSE HttpRedirect(REDIRECT_TYPE, std::string url);
    
    sessions createSession();
    void expireSession();
    RESPONSE render(std::string, inja::json = 0, bool = true);
    RESPONSE HttpResponse(std::string);
    void sendHeader();

    std::shared_ptr<darshan::network::SocketConnection> writer;
    inja::Environment env;

private:
    
    HTTP_HEADER h;
    int fd;
};
RESPONSE httpResponse::HttpResponse(std::string value)
{
    RESPONSE r;

    r.type = 0;
    r.val = value;
    r.json = 0;
    buildHeader();


    r.header = getHeader();
    return r;
}

RESPONSE httpResponse::HttpRedirect(REDIRECT_TYPE r, std::string url)
{
    setHttpStatusCode((HTTP_STATUS_CODE)r);

    set("Location", url);
    RESPONSE rmp;

    rmp.type = 0;
    rmp.json = 0;
    buildHeader();

   


    rmp.header = getHeader();
    return rmp;
};

void httpResponse::sendHeader()
{
    
    
    buildHeader();
    writer->writeString(getHeader());
};
RESPONSE httpResponse::render(std::string filename, inja::json j, bool usestaticdir)
{

    std::string v;

    if (usestaticdir == true)
    {
        v.append(DEFAULT_STATIC_DIRECTORY);
    }

    v.append(filename);
    RESPONSE res;
    res.type = 1;
    buildHeader();
   
    res.header = getHeader();
    res.val = v;
    res.json = j;

    return res;
};
sessions httpResponse::createSession()
{
    COOKIE c;
    c.name = SESSION_ID;
    c.httpOnly = true;
    c.secure = true;
    while (1)
    {

        string sid = getRandomSessionId(SESSION_VALUE_SIZE);
        string sp = SESSION_DIRECTORY;
        string s = SESSION_ID;
        s += "_";
        s += sid;
        s += ".json";
        std::filesystem::path pth(SESSION_DIRECTORY);

        pth.replace_filename(std::filesystem::path(s));


        if (!std::filesystem::exists(pth))
        {

           // fd = open(pth.generic_string().c_str(), O_RDWR | O_CREAT, S_IRWXU);
            FILE *f = fopen(pth.c_str(), "w+");

            if (f != 0)
            {

                c.value = sid;
                setCookie(c);
                fclose(f);
                return sessions( pth.string());
            }
            else
            {
                perror("error");
            }
        }
    }
};

void httpResponse::expireSession()
{
    cookieParser cp(h.cookieString);

    std::string name = SESSION_ID;

    std::vector<COOKIE> c = cp.cookies;
    COOKIE X;
    for (int i = 0; i < c.size(); i++)
    {
        if (name.compare(c.at(i).name) == 0)
        {
            X = c.at(i);
        }
    }

    if (!X.value.empty())
    {
        string sp = SESSION_DIRECTORY;
        string s = SESSION_ID;
        s += "_";
        s += X.value;
        s += ".json";
        std::filesystem::path p(SESSION_DIRECTORY);
        p.replace_filename(std::filesystem::path(s));
        bool x = remove(p);
        COOKIE c;
        c.name = SESSION_ID;
        c.httpOnly = true;
        c.secure = true;
        setCookie(c);
    }
}