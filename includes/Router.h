//bug free
#ifndef ROUTER_INCLUDED
#define ROUTER_INCLUDED

#include "enums.h"
#include <string>
#include <functional>
#include "httpRequest.h"
#include "httpResponse.h"
#include <vector>
#include "url.h"
#include "public_directory.h"

typedef std::function<bool(httpRequest, httpResponse, void *)> publicDirectoryFunction;
typedef struct __FILE_SENDER_DATA {
    std::string *baseurl;
    publicDirectoryFunction *function;
} FILE_SENDER_DATA;
typedef struct __PUB_DIR_DATA {
    string *path;
    string *dir;
    string *baseurl;

    publicDirectoryFunction *myfun;

} PUBLIC_DIR_DATA;


RESPONSE defaultCallback(httpRequest req, httpResponse res, void *) {

    res.setHttpStatusCode(HTTP_STATUS_CODE_Not_Found);

    res.setContentType(MIME_TYPE_text_html);
#ifdef DEBUG
    cout  << "Could not process for "<< req.getFullHeader().path << endl;
#endif
    return res.HttpResponse("<html><body><h1>Request Page was Not Found on This Server</h1></body></html>");
}

RESPONSE defaultPublicDirectoryFileSender(httpRequest req, httpResponse res, void *data) {

    FILE_SENDER_DATA *datax = (FILE_SENDER_DATA *) data;

    if ((datax->function) != NULL) {


        bool x = (*datax->function)(req, res, 0);

        if (!x) {
            res.setContentType(MIME_TYPE_text_html);
            res.setHttpStatusCode(HTTP_STATUS_CODE_Not_Found);

            return res.HttpResponse("<html><body>Invalid access to file</body></html>");
        }
    }
    string sx = req._GetUrlParam("s");

    if (!sx.empty()) {


        sx = decodeUrl(sx);
        std::filesystem::path p(*datax->baseurl);
        p.append(sx);

        if (std::filesystem::exists(p)) {

            res.set("Content-Description", "File Transfer");
            res.setContentType(getFileTypeFromExtension(p.string()));
            string s = "attachment; filename=\"";
            s += p.filename().string();
            s += "\"";
            res.set("Content-Disposition", s);

            res.set("Content-Length", to_string(std::filesystem::file_size(p)));

            return res.render(p.string(), 0, 0);
        }
#ifdef DEBUG
        else
            cout <<"[+] Download file "<<p.string() <<" does not exist" <<endl;
#endif

    }
    return res.HttpResponse("<html><body>Invalid access</body></html>");
}


RESPONSE defaultPublicDirectoryCallback(httpRequest req, httpResponse res, void *data) {
    PUBLIC_DIR_DATA *datax = (PUBLIC_DIR_DATA *) data;
    if ((datax->myfun) != NULL) {
        bool x = (*datax->myfun)(req, res, 0);
        res.setContentType(MIME_TYPE_text_html);

        if (!x)return res.HttpResponse("<html><body>Requested page was not found on this server</body></html>");
    }

    publicDirectory dirx(*datax->dir, res, *datax->baseurl);

    return dirx.getResponse();
}


class Router {
public:
    Router() {};

    void onRequest(std::string, std::string,bool=true);

    void onRequest(std::string, responseCallback, void *);

    void onRequest(std::string, std::string, MIME_TYPE,bool=true);

    void onDefaultRequest(responseCallback, void *);

    void installApplication(WebApplication app);

    void installPublicDirectory(std::string, std::string, publicDirectoryFunction= 0, publicDirectoryFunction= 0);

    void installStaticDirectory(std::string);

protected:
    std::vector <std::string> paths;
    std::vector <std::string> values;
    std::vector <responseCallback> callbacks;
    std::vector<void *> args;
    std::vector <MIME_TYPE> filetypes;
    std::vector <std::string> durl;
    std::vector<short> dynamicurls;
    responseCallback df = defaultCallback;
    void *dargs = 0;
};

void Router::installStaticDirectory(std::string path) {
    std::filesystem::path p(path);
    if (!exists(p) || !is_directory(p))return;
    for (const std::filesystem::directory_entry &dir_entry: filesystem::recursive_directory_iterator(path)) {
        if (dir_entry.is_regular_file()) {
            string sx = "/";
            sx += std::filesystem::relative(dir_entry, filesystem::path(path)).string();

            onRequest(sx, dir_entry.path().string(), MIME_TYPE_SELF,false);
        }
    }

}

void Router::installPublicDirectory(std::string path, std::string dir, publicDirectoryFunction dirfunction,
                                    publicDirectoryFunction downloadfunction) {


    PUBLIC_DIR_DATA *data = (PUBLIC_DIR_DATA *) malloc(sizeof(PUBLIC_DIR_DATA));
    FILE_SENDER_DATA *data2 = (FILE_SENDER_DATA *) malloc(sizeof(FILE_SENDER_DATA));

    data->path = new string(path);
    data->dir = new string(dir);
    data2->baseurl = new string(dir);

    if (dirfunction == 0) {
        data->myfun = 0;
    } else data->myfun = new publicDirectoryFunction(dirfunction);


    if (downloadfunction == 0) {
        data2->function = 0;
    } else data2->function = new publicDirectoryFunction(downloadfunction);


    string s = getRandomSessionId(8);

    std::string baseurl = "/file/";
    baseurl += s;

    data->baseurl = new string(baseurl);
    baseurl += "/";
    baseurl += "<slug:s>";

    onRequest(baseurl, defaultPublicDirectoryFileSender, (void *) data2);
    onRequest(path, defaultPublicDirectoryCallback, (void *) data);
}

void Router::onRequest(std::string path, std::string value,bool usedefaultdirectory) {
    paths.push_back(path);
    if(usedefaultdirectory){
        string s = DEFAULT_STATIC_DIRECTORY;
        s += "/";
        s +=value;
        values.push_back(s);
    }
    else values.push_back(value);
    filetypes.push_back(MIME_TYPE_SELF);
    callbacks.push_back(0);
    args.push_back(0);
    dynamicurls.push_back(0);
    durl.push_back(std::string());
}

void Router::onRequest(std::string path, std::string value, MIME_TYPE m,bool usedefaultdirectory) {

    paths.push_back(path);
    if(usedefaultdirectory){
        string s = DEFAULT_STATIC_DIRECTORY;
        s += "/";
        s +=value;
        values.push_back(s);
    }
    else values.push_back(value);
    filetypes.push_back(m);
    callbacks.push_back(0);
    args.push_back(0);
    dynamicurls.push_back(0);
    durl.push_back(std::string());
}

void Router::onRequest(std::string path, responseCallback callback, void *x = 0) {
    if (isDynamicUrl(path)) {
        dynamicurls.push_back(1);
        string s = getDynamicBasicString(path);

        paths.push_back(s);
        durl.push_back(path);

    } else {
        dynamicurls.push_back(0);
        paths.push_back(path);
        durl.push_back(string());
    }

    values.push_back(std::string());
    filetypes.push_back(MIME_TYPE_SELF);
    callbacks.push_back(callback);
    args.push_back(x);

}

void Router::onDefaultRequest(responseCallback callback, void *args) {
    df = callback;
    dargs = args;
}

void Router::installApplication(WebApplication app) {

    ROUTER_DATA d = app.getRouterData();

    for (int i = 0; i < app.types.size(); i++) {

        if (app.types.at(i) == true) {
            onRequest(d.paths.at(i), d.callbacks.at(i), d.args.at(i));
        } else {
            onRequest(d.paths.at(i), d.values.at(i), d.filetypes.at(i));
        }
    }
}

#endif