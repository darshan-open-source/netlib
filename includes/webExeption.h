// bug free
#pragma once

#include <httpResponse.h>

class webException {
public:
    webException(httpResponse, HTTP_STATUS_CODE);

    void setContent(std::string);
    void setContentFile(std::string);

    RESPONSE sendException();

private:
    httpResponse resm;
    bool isText = false;
    std::string content;
};
