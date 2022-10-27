#include <iostream>
#include "../includes/webserver.h"

using namespace std;

RESPONSE login(httpRequest req, httpResponse res, void *)
{


    return res.HttpResponse("login successfull");
}
RESPONSE logout(httpRequest req, httpResponse res, void *)
{

   
    return res.HttpResponse("logout successfull");
}

int main()
{
    webserver server("127.0.0.1", 5002);

    WebApplication app("/admin/");

    app.onRequest("login", login);
    app.onRequest("logout", logout);



    server.installApplication(app);

    server.start();
    server.wait();
}