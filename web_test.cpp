#include <iostream>
#include "includes/webserver.h"
#include "includes/public_directory.h"
using namespace std;

RESPONSE admin(httpRequest req, httpResponse res, void *)
{

    return res.HttpResponse("hello from admin");
}
RESPONSE renderex(httpRequest req, httpResponse res, void *)
{
    cout << "render ex 2"<<endl;
    res.setContentType(MIME_TYPE_text_html);
    res.setConnectionType(CONNECTION_TYPE::CONNECTION_TYPE_KEEP_ALIVE);
    res.setCookie("cookie=true");
    res.setServer("darshan web services");
    res.setString("anything: else\r\n");
    res.setHttpVersion(HTTP_VERSION_HTTP2);
    res.setDate();

    inja::json j;
    j["name"] = "darshan";
    j["surname"] = "sojitra";
    j["guests"] = {"person1", "person2", "person2"};
    j["time"]["hour"] = 30;
    j["time"]["minute"] = 60;

    try
    {

        return res.render("template.html", j);
    }
    catch (std::exception &e)
    {
        return res.HttpResponse("hello");
    }
}
RESPONSE getimg(httpRequest req, httpResponse res, void *)
{

    return res.render("/home/darshan/Documents/code/netlib(main)/default_statics/file.jpeg", 0, false);
}
RESPONSE login(httpRequest req, httpResponse res, void *)
{
    
    if (req.isSessionSet())
    {
        std::string s = req.getSession().get("username");
        s+= " ";
        s+= req.getSession().get("password");
        return res.HttpResponse(s);
    }
    else
    {
        res.createSession();
        return res.HttpResponse("session is created successfully");
    }
}
RESPONSE logout(httpRequest req, httpResponse res, void *)
{

    res.expireSession();

    return res.HttpResponse("session expired successfully");
}
RESPONSE action(httpRequest req, httpResponse res, void *)
{

    inja::json j;
    j["username"] = req._GET_MULTIPART("username").value;
    j["password"] = req._GET_MULTIPART("password").value;

    j["file1"] = req._GET_MULTIPART("file1").isfilename;
    j["file2"] =  req._GET_MULTIPART("file2").isfilename;

     if(!req.isSessionSet()) {
         //
         sessions s = res.createSession();


         s.set("username", req._GET("username"));
         s.set("password", req._GET("password"));
     }
     else{
         return res.render("disp.html",j);
     }

    return res.render("disp.html", j);
};
RESPONSE datax(httpRequest req, httpResponse res, void *)
{
    inja::json j;
    j["a"]   = req._GetUrlParam("a");
    j["b"]   = req._GetUrlParam("b");
    j["s"]   = req._GetUrlParam("s");
    j["an"]  = req._GetUrlParam("an");
    j["a1"]  = req._GetUrlParam("a1");
    j["b1"]  = req._GetUrlParam("b1");
    j["s1"]  = req._GetUrlParam("s1");
    j["an1"] = req._GetUrlParam("an1");
    j["p"]   = req._GetUrlParam("p");
   
    return res.render("dpath.html", j);
}
RESPONSE redirect(httpRequest req, httpResponse res, void *)
{
    std::string s = "https://";
    s +=  req._GetUrlParam("s");
    cout << "layer "<<endl;
    return res.HttpRedirect(REDIRECT_TYPE_FOUND,s);
}
RESPONSE handlecookie(httpRequest req,httpResponse res,void *){
    if(!req._GET("cookiestr").empty() && !req._GET("cookieval").empty()){




        res.setCookie(req._GET("cookiestr"),req._GET("cookieval"));
        return res.HttpResponse("<html><body> cookie successfully get</body></html>");
    }

    return  res.HttpResponse("<html> <body> Invalid cookie </body></html>");
}
RESPONSE getcookie(httpRequest req,httpResponse res,void *) {
    inja::json j;
    std::vector<std::string> c1,c2;
    for (int i = 0; i < req.getAllCookies().size(); i++) {
        c1.push_back(req.getAllCookies().at(i).name);
        c2.push_back(req.getAllCookies().at(i).value);
    }
     
    cout  << c1.size()<<endl;
    string arr[100];
    std::copy(c1.begin(), c1.end(), arr);
    string arr2[100];
    std::copy(c2.begin(), c2.end(), arr2);
    j["cookiestr"] = arr;
    j["cookieval"] = arr2;
    j["rnx"] = c1.size();



    return res.render("dispcookie.html",j);
}

bool downloadfun(httpRequest,httpResponse,void*){
    return true;
}
bool pubdirfun(httpRequest,httpResponse,void*){
    return true;
}
RESPONSE a(httpRequest request,httpResponse response,void*){
    cout << "data found "<< request._GetUrlParam("a") <<"  "<< request._GetUrlParam("b") <<"  "<< request._GetUrlParam("c") << endl;
    cout << " " << request._GetUrlParam("p")<<endl;
    return  response.HttpResponse("found");
}
RESPONSE p(httpRequest request,httpResponse response,void*){
    cout << "data found "<< request._GetUrlParam("p") <<endl;

    return  response.HttpResponse("found");
}
int main()
{

    webserver s("127.0.0.1", 8085);
    s.onRequest("/", "index.html");
    s.onRequest("/admin", admin);
    s.onRequest("/render", renderex);
    s.onRequest("/getimage", "/home/darshan/Documents/code/netlib(main)/default_statics/file.jpeg", MIME_TYPE_image_jpeg,false);
    s.onRequest("/login", login);
    s.onRequest("/logout", logout);
    s.onRequest("/action", action);
    s.onRequest("/redirect/<slug:s>",redirect);
    s.onRequest("/form", "form.html");
    s.onRequest("/setcookie","cookieform.html");
    s.onRequest("/setcookiex",handlecookie);
    s.onRequest("/getcookie",getcookie);
    s.installStaticDirectory(BUILTIN_STATIC_DIRECTORY);
    s.installPublicDirectory("/pubdir","/home/darshan/Documents/code/netlib(main)/includes/",pubdirfun,downloadfun);
    s.onRequest("/data1/<int :a>/<float:b>/<string:s>/<alphanum:an>/datax/2/<int :a1>/<float:b1>/<string:s1>/<alphanum:an1>/", datax);
    s.onRequest("/id/<string:a>/<int :b>/<float:c>/mydata/<path:p>/ssl",a);
    s.onRequest("/pdata/<path:p>/over",p);
    s.start();
    s.wait();
}