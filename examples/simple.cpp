#include<iostream>
#include "../includes/webserver.h"

using namespace std;

RESPONSE home(httpRequest req,httpResponse res,void*){
    
    
    return res.HttpResponse("This is Home page");
}
int main(){
        webserver server("127.0.0.1",5002);
        server.onRequest("/",home);
        server.onRequest("/about","about.html");

        server.start();
        server.wait();
}