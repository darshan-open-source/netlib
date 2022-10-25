// bug free
#pragma once

#include "httpResponse.h"

class webException {
public:
    webException(httpResponse, HTTP_STATUS_CODE);

    void setContent(std::string);
    void setContentFile(std::string);

    RESPONSE sendException();

private:
    httpResponse resm;
    bool iscontent = true;
    std::string content;
};

webException::webException(httpResponse res, HTTP_STATUS_CODE status) : resm(res) {
    resm.setHttpStatusCode(status);

}

void webException::setContent(std::string str) {
    content = str;
    iscontent = true;
}

void webException::setContentFile(std::string str) {

    content = str;
    iscontent = false;
}

RESPONSE webException::sendException() {
    RESPONSE r;
    r.val = content;
    if (iscontent == true) {

        resm.setContentLength(content.length());
        r.type = 0;

    } else {


        resm.setContentLength(std::filesystem::file_size(std::filesystem::path(content)));
        r.type = 1;
    }
    r.header = resm.getresponse();

    return r;
}
