// bug free
#ifndef ROUTER_INCLUDED
#define ROUTER_INCLUDED

#include <vector>
#include <functional>
#include <string>
#include <enums.h>
#include <httpRequest.h>
#include <httpResponse.h>
#include <url.h>
#include <public_directory.h>
#include<Application.h>
#include<response_header.h>
typedef std::function<bool(httpRequest, httpResponse, void *)> publicDirectoryFunction;
typedef struct __FILE_SENDER_DATA
{
    std::string *baseurl;
    publicDirectoryFunction *function;
} FILE_SENDER_DATA;
typedef struct __PUB_DIR_DATA
{
    string *path;
    string *dir;
    string *baseurl;

    publicDirectoryFunction *myfun;

} PUBLIC_DIR_DATA;

RESPONSE defaultCallback(httpRequest req, httpResponse res, void *);
class Router
{
public:
    Router(){};

    void onRequest(std::string, std::string, bool = true);

    void onRequest(std::string, responseCallback, void *);

    void onRequest(std::string, std::string, MIME_TYPE, bool = true);

    void onDefaultRequest(responseCallback, void *);

    void installApplication(WebApplication);

    void installPublicDirectory(std::string, std::string, publicDirectoryFunction = 0, publicDirectoryFunction = 0);

    void installStaticDirectory(std::string);

protected:
    std::vector<std::string> paths;
    std::vector<std::string> values;
    std::vector<responseCallback> callbacks;
    std::vector<void *> args;
    std::vector<MIME_TYPE> filetypes;
    std::vector<std::string> durl;
    std::vector<bool> dynamicurls;
    responseCallback df = defaultCallback;
    void *dargs = 0;
};

#endif