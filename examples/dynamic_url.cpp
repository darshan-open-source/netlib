#include <iostream>
#include "../includes/webserver.h"

using namespace std;

RESPONSE intx(httpRequest req, httpResponse res, void *)
{

    std::string sx = req._GetUrlParam("a");

    return res.HttpResponse(sx);
}
RESPONSE function1(httpRequest req, httpResponse res, void *)
{

    std::string sx = req._GetUrlParam("a");
    std::string sx2 = req._GetUrlParam("b");
    std::string sx3 = req._GetUrlParam("p");

    return res.HttpResponse(sx);
}
RESPONSE floatx(httpRequest req, httpResponse res, void *)
{

    std::string sx = req._GetUrlParam("floatx");

    return res.HttpResponse(sx);
}
int main()
{
    webserver server("127.0.0.1", 5002);

    server.onRequest("/price/<int:a>", intx);
    server.onRequest("/<int:a>/name/<string:b>/path/<path:p>/", function1);
    server.onRequest("/decimal/<float:b>", floatx);

    server.start();
    server.wait();
}