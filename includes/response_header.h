//bug free
#ifndef RH_INCLUDED
#define RH_INCLUDED
#include <string>
#include <enums.h>
#include <server_time.h>
#include<stringutil.h>
class responseHeaderBuilder
{
public:
    responseHeaderBuilder();
    responseHeaderBuilder(HTTP_VERSION, HTTP_STATUS_CODE);
    void setHttpVersion(HTTP_VERSION);
    void setHttpStatusCode(HTTP_STATUS_CODE);
    void setDate();
    void setContentLength(size_t);
    void set(std::string, std::string);
    void setServer(std::string);
    void setContentEncoding(CONTENT_ENCODING);
    void setConnectionType(CONNECTION_TYPE);
    void setContentType(MIME_TYPE, ENCODING=ENCODING_UNDEFINED);
    void setCookie(COOKIE);
    void setCookie(std::string, std::string);
    std::string getHeader();
protected:
    void buildHeader(){};
    HTTP_RESPONSE resp_object;
private:
    std::string header;
    void addHeader(std::string,std::string);
};

#endif