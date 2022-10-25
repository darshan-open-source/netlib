// bug free
#pragma once

struct __COOKIE;

#ifndef SESSION_ID
#define SESSION_ID "SESSION_ID"
#endif

#ifndef SESSION_VALUE_SIZE
#define SESSION_VALUE_SIZE 12
#endif

#ifndef SESSION_DIRECTORY
#define SESSION_DIRECTORY "/home/darshan/Documents/code/netlib(main)/sessions/"
#endif

#include "cookies.h"
#include <filesystem>
#include "sessions.h"
#include "url.h"

class httpRequest {

public:
    httpRequest(HTTP_HEADER, std::vector <URL_DATA> udata);

    httpRequest() {};

    std::string _GET(std::string);

    MULTIPART_DATA _GET_MULTIPART(std::string);

    COOKIE getCookie(std::string);

    std::string getCookieString(std::string);

    std::vector <COOKIE> getAllCookies();

    HTTP_HEADER getFullHeader();
    std::vector<URL_DATA> getFullUrlData(){return urldatas;}
    bool isSessionSet();

    std::string _GetUrlParam(std::string);

    sessions getSession();

    std::string sessionFile;

private:
    std::vector <COOKIE> cookies;
    std::multimap <std::string, std::string> payload;
    HTTP_HEADER h;
    std::vector <URL_DATA> urldatas;
};

MULTIPART_DATA httpRequest::_GET_MULTIPART(std::string name) {
    for (int i = 0; i < getFullHeader().multipart_data.size(); i++) {
        if (getFullHeader().multipart_data.at(i).name.compare(name) == 0) {
            return getFullHeader().multipart_data.at(i);
        }
    }
    MULTIPART_DATA md;
    return md;
}

httpRequest::httpRequest(HTTP_HEADER header, std::vector <URL_DATA> udata) : urldatas(udata), payload(header.payload) {
    h = header;
    cookieParser p(h.cookieString);
    cookies = p.cookies;

}

std::vector <COOKIE> httpRequest::getAllCookies() {
    return cookies;
}

COOKIE httpRequest::getCookie(std::string name) {

    for (int i = 0; i < cookies.size(); i++) {
        if (name.compare(cookies.at(i).name) == 0) {
            return cookies.at(i);
        }
    }
    COOKIE tc;

    return tc;
}

std::string httpRequest::getCookieString(std::string name) {
    COOKIE ck = getCookie(name);
    if (!ck.name.empty()) {
        return ck.value;
    }
    std::string tc;

    return tc;
}

std::string httpRequest::_GET(std::string name) {
    std::multimap<std::string, std::string>::iterator it;
    for (it = payload.begin(); it != payload.end(); it++) {
        if (it->first.compare(name) == 0) {
            return it->second;
        }
    }
    string s;
    return s;
}

HTTP_HEADER httpRequest::getFullHeader() {
    return h;
}

bool httpRequest::isSessionSet() {

    string cookieval = getCookieString(SESSION_ID);
    if (!cookieval.empty() && cookieval.length() == SESSION_VALUE_SIZE) {
        string s = SESSION_ID;
        s += "_";
        s += cookieval;
        s += ".json";
        std::filesystem::path p(SESSION_DIRECTORY);
        p.replace_filename(std::filesystem::path(s));
        if (std::filesystem::exists(p)) {

            sessionFile = p.generic_string();
            return true;
        }
    }
    return false;
}

sessions httpRequest::getSession() {
    if (isSessionSet()) {
        return sessions(sessionFile);
    }
    return sessions();
}

std::string httpRequest::_GetUrlParam(std::string name) {

    for (int i = 0; i < urldatas.size(); i++) {
        if (urldatas.at(i).name.compare(name) == 0) {
            return urldatas.at(i).value;
        }
    }
    string s;
    return s;
}