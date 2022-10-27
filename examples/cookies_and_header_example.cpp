#include <iostream>
#include "../includes/webserver.h"

using namespace std;

RESPONSE setcookie(httpRequest req, httpResponse res, void *)
{

  res.setDate();
  res.setServer("example servers");
  res.setConnectionType(CONNECTION_TYPE_KEEP_ALIVE);

  res.setCookie("name=darshan");
  return res.HttpResponse("cookie is successfully set");
}

RESPONSE getcookie(httpRequest req, httpResponse res, void *)
{
  nlohmann::json j;
  j["cookie"] = req.getCookieString("name");

  return res.render("getcookie.html", j);
}

int main()
{
  webserver server("127.0.0.1", 5002);

  server.onRequest("/", setcookie);
  server.onRequest("/getcookie", getcookie);

  server.start();
  server.wait();
}