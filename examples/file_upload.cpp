#include<iostream>
#include "../includes/webserver.h"

using namespace std;

RESPONSE action(httpRequest req,httpResponse res,void*){
    
     
    MULTIPART_DATA  username = req._GET_MULTIPART("username");
    MULTIPART_DATA password = req._GET_MULTIPART("password");
    MULTIPART_DATA file = req._GET_MULTIPART("file");

    cout << "username is "<<username.value << " password is "<<password.value << endl;
    cout << "file "<< file.isfilename << "sucessfully uploaded"<<endl;
    return res.HttpResponse("file successfully uploaded");
}
int main(){
        webserver server("127.0.0.1",5002);


        server.onRequest("/","file_upload.html");
        server.onRequest("/action",action);

        server.start();
        server.wait();
}