#pragma once
#ifndef BLOCK_SIZE
#define BLOCK_SIZE 100000
#endif

#ifndef MAX_HEADER_LEN
#define MAX_HEADER_LEN 8000
#endif

#ifndef MAX_PAYLOAD_SIZE
#define MAX_PAYLOAD_SIZE 3221225472
#endif

#include <settings.h>
#include<config.h>
// static std::string session_directory = readValueFromDefaultConfig("session_directory")  ;


#include <iostream>
#include <enums.h>

#ifdef USE_SSL
#include <SecureSocketServer.h>
#else
#include <SocketServer.h>
#endif

#include <header_processor2.h>
#include <Application.h>
#include <dynamicurl.h>
#include <debug.h>
#include <Router.h>
#include <helper.h>
#include <test_middleware.h>

using namespace std;

class webserver : public darshan::network::SocketServer, public Router
{
public:
    webserver(const char *host, int port) : darshan::network::SocketServer((char *)host, port)
    {

       middleware.push_back(new defaultMiddleware());
       middleware.push_back(new DecodeData());
        if (!std::filesystem::exists(std::filesystem::path(SESSION_DIRECTORY)))
        {
            std::filesystem::create_directories(SESSION_DIRECTORY);
        }
    };

    void onClientConnected(shared_ptr<darshan::network::SocketConnection> s);

private:
    dynamicPath p;
    void process(header_processor2 *, shared_ptr<darshan::network::SocketConnection>);
    unsigned long long needRead = 0;

    void sendDirect(int, shared_ptr<darshan::network::SocketConnection> s);

    void sendRes(shared_ptr<darshan::network::SocketConnection> s, RESPONSE res);

    timeval vx = {4, 0};
    int timeout = 4000;
};
