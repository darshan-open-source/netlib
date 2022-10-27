#include<iostream>
#include "../includes/webserver.h"

using namespace std;
class mymiddleware : public Middleware{
    public:
    mymiddleware(){};
    RESPONSE __call__(httpRequest req,httpResponse res,DEFAULT_DATA not_usable){


        HTTP_HEADER x = req.getFullHeader();
        std::vector<URL_DATA> x2 = req.getFullUrlData();
        

        //request is successfully intercepted by middleware
        x.host = "www.google.com";
       

        httpRequest reqx(x,x2);

        
        return sendResponse(reqx,res,not_usable);
    }
};
int main(){
        webserver server("127.0.0.1",5002);

        server.installMiddleware(new mymiddleware());
        server.onRequest("/","index.html");

        server.start();
        server.wait();
}