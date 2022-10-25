// bug free
#pragma once
#include "standalone.h"
#include "url.h"

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

void WebApplication::onRequest(std::string path, std::string value)
{
    string s = urlx;
    s.append(path);
    types.push_back(false);
    paths.push_back(s);
    values.push_back(value);
    filetypes.push_back(MIME_TYPE_SELF);
    callbacks.push_back(0);
    args.push_back(0);
    dynamicurls.push_back(0);
    durl.push_back(std::string());
}
void WebApplication::onRequest(std::string path, std::string value, MIME_TYPE m)
{
    string s = urlx;
    s.append(path);
    types.push_back(false);

    paths.push_back(s);
    values.push_back(value);
    filetypes.push_back(m);
    callbacks.push_back(0);
    args.push_back(0);
    dynamicurls.push_back(0);
    durl.push_back(std::string());
}
void WebApplication::onRequest(std::string path, responseCallback callback, void *x = 0)
{
    string s = urlx;
    s.append(path);
    types.push_back(true);
    if (isDynamicUrl(path))
    {
        dynamicurls.push_back(1);
        string s = getDynamicBasicString(path);

        paths.push_back(s);
        durl.push_back(path);
    }
    else
    {
        dynamicurls.push_back(0);
        paths.push_back(s);
        durl.push_back(std::string());
    }

    values.push_back(std::string());
    filetypes.push_back(MIME_TYPE_SELF);
    callbacks.push_back(callback);
    args.push_back(x);

    durl.push_back(path);
}
void WebApplication::installApplication(WebApplication app)
{

    ROUTER_DATA d = app.getRouterData();

    for (int i = 0; i < app.types.size(); i++)
    {

        if (app.types.at(i) == true)
        {
            onRequest(d.paths.at(i), d.callbacks.at(i), d.args.at(i));
        }
        else
        {
            onRequest(d.paths.at(i), d.values.at(i), d.filetypes.at(i));
        }
    }
}