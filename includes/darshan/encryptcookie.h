#pragma once
#include "../middleware.h"

class EncryptCookies : public Middleware
{

public:
    EncryptCookies() : Middleware()
    {
    }

protected:
    void run(httpRequest)
    {
        sendResponse();
    };
};
