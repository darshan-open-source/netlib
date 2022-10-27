#include <iostream>
#include "../includes/webserver.h"

using namespace std;

bool onrequest(httpRequest req, httpResponse res, void *)
{
     return true ;// return true for accessing file
 
}
bool ondownload(httpRequest req, httpResponse res, void *)
{

   
    return true;//return true for download the file and false prevents the download 
}

int main()
{
    webserver server("127.0.0.1", 5002);
    
    server.installStaticDirectory("/home/static/");
    server.installPublicDirectory("/pubdir","/path/to/files/");
    server.installPublicDirectory("/pubdir2","/path/to/file",onrequest,ondownload);

    server.start();
    server.wait();
}