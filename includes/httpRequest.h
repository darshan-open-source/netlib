// bug free
#pragma once

struct __COOKIE;

#ifndef SESSION_ID
#define SESSION_ID "SESSION_ID"
#endif

#ifndef SESSION_VALUE_SIZE
#define SESSION_VALUE_SIZE 12
#endif


#include <enums.h>

#include <cookies.h>
#include <filesystem>
#include <sessions.h>
#include <url.h>
#include<config.h>

static std::string session_directory = readValueFromDefaultConfig("session_directory")  ;

class httpRequest
{

public:
    httpRequest(HTTP_HEADER, std::vector<URL_DATA> udata);

    httpRequest(){};

    std::string _GET(std::string);
    std::string _POST(std::string);
    std::string _REQUEST(std::string);

    MULTIPART_DATA _GET_MULTIPART(std::string);

    std::string getCookie(std::string);
    std::string getOtherPayload();

    std::vector<COOKIE> getAllCookies();

    HTTP_HEADER getFullHeader();
    std::vector<URL_DATA> getFullUrlData() { return urldatas; }
    bool isSessionSet();

    std::string _GetUrlParam(std::string);

    sessions getSession();

    std::string sessionFile;

private:
    std::vector<COOKIE> cookies;
    HTTP_HEADER h;
    std::vector<URL_DATA> urldatas;
};
