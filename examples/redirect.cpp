#include<iostream>
#include "../includes/webserver.h"

using namespace std;

RESPONSE redirect(httpRequest req,httpResponse res,void*){
    
     
    return res.HttpRedirect(REDIRECT_TYPE_MOVED_PERMANENTLY,"https://www.google.com");
}
int main(){
        webserver server("127.0.0.1",5002);
        server.onRequest("/",redirect);

        server.start();
        server.wait();
}