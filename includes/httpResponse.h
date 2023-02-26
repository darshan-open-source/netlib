// bug free
#pragma once

#include <memory>
#include <SocketServer.h>
#include <enums.h>
#include <response_header.h>
#include <httpRequest.h>
#include <sessions.h>
#include <filesystem>
#include<standalone.h>
#include <inja/inja.hpp>
#include<config.h>
static std::string defaultStaticDirectory = readValueFromDefaultConfig("default_static_directory")  ;


class httpResponse : public responseHeaderBuilder
{
public:
    httpResponse(std::shared_ptr<darshan::network::SocketConnection> s, HTTP_HEADER x) : writer(s) { h = x; };
    httpResponse(){};

    sessions createSession();
    void expireSession();
    RESPONSE HttpRedirect(REDIRECT_TYPE, std::string url);
    RESPONSE HttpResponse(std::string);
    RESPONSE RawResponse(std::string);
    RESPONSE render(std::string, inja::json = 0, bool = true);
    void sendHeader();

    std::shared_ptr<darshan::network::SocketConnection> writer;
    inja::Environment env;

private:
    HTTP_HEADER h;
    int fd;
};
