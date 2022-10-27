#include<iostream>
#include "../includes/webserver.h"

using namespace std;

RESPONSE action(httpRequest req,httpResponse res,void*){
    
     
    std::string username = req._GET("username");
    std::string password = req._GET("password");


    cout << "username is "<<username << " password is "<<password << endl;

    return res.HttpResponse(username);
}
int main(){
        webserver server("127.0.0.1",5002);


        server.onRequest("/","index.html");
        server.onRequest("/action",action);

        server.start();
        server.wait();
}