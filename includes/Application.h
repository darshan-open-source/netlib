// bug free

//currently not usable
#pragma once
#include <standalone.h>
#include <url.h>

class httpRequest;
class httpResponse;
typedef std::function<RESPONSE(httpRequest, httpResponse, void *)> responseCallback;
typedef struct _ROUTER_DATA
{
    std::vector<std::string> paths;
    std::vector<std::string> values;
    std::vector<responseCallback> callbacks;
    std::vector<void *> args;
    std::vector<MIME_TYPE> filetypes;
    std::vector<std::string> durl;
    std::vector<short> dynamicurls;

} ROUTER_DATA;
class WebApplication
{
public:
    WebApplication(std::string baseUrl) : urlx(baseUrl){};
    void onRequest(std::string, std::string);
    void onRequest(std::string, responseCallback, void *);
    void onRequest(std::string, std::string, MIME_TYPE);
    void installApplication(WebApplication app);

    // void onRequest(std::string url, responseCallback callback, void *obj)
    // {
    //     string s = urlx;
    //     s.append(url);

    //     Router::onRequest(s, callback, obj);
    //     types.push_back(true);
    // };
    // void onRequest(std::string url, std::string value)
    // {
    //     string s = urlx;
    //     s.append(url);

    //     Router::onRequest(s, value,MIME_TYPE_SELF);

    //     types.push_back(false);
    // };
    // void onRequest(std::string url, std::string value, MIME_TYPE mime_type)
    // {

    //     string s = urlx;
    //     s.append(url);
    //     types.push_back(false);

    //     Router::onRequest(s, value, mime_type);
    // };
    ROUTER_DATA getRouterData()
    {
        ROUTER_DATA rd;

        rd.paths = paths;
        rd.args = args;
        rd.callbacks = callbacks;
        rd.values = values;
        rd.filetypes = filetypes;
        rd.durl = durl;
        rd.dynamicurls = dynamicurls;

        return rd;
    }

    std::vector<bool> types;

private:
    std::string urlx;
    std::vector<std::string> paths;
    std::vector<std::string> values;
    std::vector<responseCallback> callbacks;
    std::vector<void *> args;
    std::vector<MIME_TYPE> filetypes;
    std::vector<std::string> durl;
    std::vector<short> dynamicurls;
};
 