// #include<enums.h>
#include <middleware.h>

class httpMethodMiddleware : public Middleware
{
public:
    httpMethodMiddleware(std::vector<HTTP_METHODS>);
    RESPONSE __call__(httpRequest, httpResponse, DEFAULT_DATA);
    private:
    std::vector<HTTP_METHODS> avaliable_http_methods;
};

class defaultMiddleware : public Middleware
{
public:
    RESPONSE __call__(httpRequest req, httpResponse res, DEFAULT_DATA not_usable) { return sendResponse(req, res, not_usable); };
};