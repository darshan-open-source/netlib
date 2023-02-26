#include<middleware.h>
Middleware::Middleware(){
    
}

 RESPONSE Middleware::sendResponse(httpRequest req,httpResponse res,DEFAULT_DATA not_usable)
    {

        if (not_usable.mlm == middleware.size()-1)
        {

            RESPONSE r;
            r = not_usable.callback(req,res,not_usable.args);
            return r;
        }
        else
        {
            // if(middleware.size()==0){
            //     this->middleware = not_usable.mw;
            // }
            // else not_usable.mw = middleware;
            not_usable.mlm +=  1;
            RESPONSE m = middleware.at(not_usable.mlm)->__call__(req,res,not_usable);
            return m;
        }
    };