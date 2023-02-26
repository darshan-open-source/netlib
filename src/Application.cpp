#include<Application.h>
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