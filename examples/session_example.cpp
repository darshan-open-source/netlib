#include<iostream>
#include "../includes/webserver.h"

using namespace std;

RESPONSE setcookie(httpRequest req,httpResponse res,void*){
  
     res.setCookie("name=darshan");
  return res.HttpResponse("cookie is successfully set");
}

RESPONSE setsession(httpRequest req,httpResponse res,void*){
    
    
   if(!req.isSessionSet())
     {
       sessions s =  res.createSession();
        s.set("name","darshan");
     }

     

  return res.HttpResponse("session successfully set");
}

RESPONSE getsession(httpRequest req,httpResponse res,void*){
    
    
   if(req.isSessionSet())
     {
       sessions s =  req.getSession();
       std::string x = s.get("name");
       return res.HttpResponse(x);
     }

     

  return res.HttpResponse("session is not set");
}


RESPONSE expire_session(httpRequest req,httpResponse res,void*){
    
    
    res.expireSession();

  return res.HttpResponse("session is expired");
}
int main(){
        webserver server("127.0.0.1",5002);


        server.onRequest("/",setsession);
        server.onRequest("/getsession",getsession);
        server.onRequest("/logout",expire_session);

        server.start();
        server.wait();
}