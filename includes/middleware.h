// bug free
#pragma once
#include <httpRequest.h>
#include <httpResponse.h>

  inline std::vector<Middleware*> middleware;
class Middleware
{

public:
    Middleware();
    virtual RESPONSE __call__(httpRequest req,httpResponse res, DEFAULT_DATA  not_usable) = 0;
protected:
    
    RESPONSE sendResponse(httpRequest req,httpResponse res,DEFAULT_DATA not_usable);

};


class defaultMiddleware:public Middleware{
public:
    RESPONSE __call__(httpRequest req,httpResponse res, DEFAULT_DATA  not_usable){return sendResponse(req,res,not_usable);};

};