// bug free
#pragma once
#include "httpRequest.h"

class Middleware;
std::vector<Middleware*> middleware;

class Middleware
{

public:
    Middleware(){};
    virtual RESPONSE __call__(httpRequest req,httpResponse res, DEFAULT_DATA  not_usable) = 0;
protected:
    
    RESPONSE sendResponse(httpRequest req,httpResponse res,DEFAULT_DATA not_usable)
    {

        if (not_usable.mlm == middleware.size()-1)
        {

            RESPONSE r;
            r = not_usable.callback(req,res,not_usable.args);
            return r;
        }
        else
        {

            not_usable.mlm +=  1;
            RESPONSE m = middleware.at(not_usable.mlm)->__call__(req,res,not_usable);
            return m;
        }
    };


};


class defaultMiddleware:public Middleware{
public:
    RESPONSE __call__(httpRequest req,httpResponse res, DEFAULT_DATA  not_usable){return sendResponse(req,res,not_usable);};

};