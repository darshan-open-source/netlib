#include <iostream>
#include "includes/webserver.h"
#include "includes/public_directory.h"
using namespace std;

RESPONSE admin(httpRequest req, httpResponse res, void *)
{

    return res.HttpResponse("hello from admin");
}

int main()
{

    webserver s("127.0.0.1", 8085);
    s.onRequest("/", "index.html");
   
    s.startServer();
    s.runServer();
}
