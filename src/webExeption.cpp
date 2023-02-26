#include<webExeption.h>
webException::webException(httpResponse res, HTTP_STATUS_CODE status) : resm(res) {
    resm.setHttpStatusCode(status);

}

void webException::setContent(std::string str) {
    content = str;
    isText = true;
}

void webException::setContentFile(std::string str) {

    content = str;
}

RESPONSE webException::sendException() {
    RESPONSE r;
    r.json=0;
    r.val = content;
    if (isText == true) {

        resm.setContentLength(content.length());
        r.type = 0;

    } else {


        resm.setContentLength(std::filesystem::file_size(content));
        r.type = 1;
    }
    // resm.buildHeader();
    r.header = resm.getHeader();
     cout << "at this point header is " << resm.getHeader();
    return r;
}

webException::webException(httpResponse res, HTTP_STATUS_CODE status) : resm(res) {
    resm.setHttpStatusCode(status);

}

void webException::setContent(std::string str) {
    content = str;
    isText = true;
}

void webException::setContentFile(std::string str) {

    content = str;
}

RESPONSE webException::sendException() {
    RESPONSE r;
    r.json=0;
    r.val = content;
    if (isText == true) {

        resm.setContentLength(content.length());
        r.type = 0;

    } else {


        resm.setContentLength(std::filesystem::file_size(content));
        r.type = 1;
    }
    // resm.buildHeader();
    r.header = resm.getHeader();
     cout << "at this point header is " << resm.getHeader();
    return r;
}

webException::webException(httpResponse res, HTTP_STATUS_CODE status) : resm(res) {
    resm.setHttpStatusCode(status);

}

void webException::setContent(std::string str) {
    content = str;
    isText = true;
}

void webException::setContentFile(std::string str) {

    content = str;
}

RESPONSE webException::sendException() {
    RESPONSE r;
    r.json=0;
    r.val = content;
    if (isText == true) {

        resm.setContentLength(content.length());
        r.type = 0;

    } else {


        resm.setContentLength(std::filesystem::file_size(content));
        r.type = 1;
    }
    // resm.buildHeader();
    r.header = resm.getHeader();
     cout << "at this point header is " << resm.getHeader();
    return r;
}
