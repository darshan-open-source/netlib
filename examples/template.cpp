// for more information on template go to https://github.com/pantor/inja

#include <iostream>
#include "../includes/webserver.h"

using namespace std;

RESPONSE templatex(httpRequest req, httpResponse res, void *)
{
        nlohmann::json jsonx;

        jsonx["name"] = "darshan";
        jsonx["surname"] = "patel";
        jsonx["guests"] = {"david", "james", "donald"};
        jsonx["time"]["hour"] = 30;
        jsonx["time"]["minute"] = 60;

        return res.render("template.html", jsonx);
}
int main()
{
        webserver server("127.0.0.1", 5002);

        server.onRequest("/", templatex);

        server.start();
        server.wait();
}
