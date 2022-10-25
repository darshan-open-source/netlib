//bug free
#ifndef RH_INCLUDED
#define RH_INCLUDED
#include <string>
#include "enums.h"
#include "server_time.h"

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
    void setString(std::string);
    void setServer(std::string);
    void setContentEncoding(CONTENT_ENCODING);
    void setConnectionType(CONNECTION_TYPE);
    void setContentType(MIME_TYPE, ENCODING);
    void setCookie(COOKIE);
    void setCookie(std::string);
    void setCookie(std::string, std::string);
    std::string getHeader() { return header; }
protected:
    void buildHeader();

private:
    std::string header;
};
void responseHeaderBuilder::setContentLength(size_t length)
{
    set("Content-Length",to_string(length));
}

void responseHeaderBuilder::buildHeader()
{
    header.append("\r\n");
}
void responseHeaderBuilder::setString(std::string string)
{
    header.append(string);
}
void responseHeaderBuilder::setConnectionType(CONNECTION_TYPE t)
{
    if (t == CONNECTION_TYPE_KEEP_ALIVE)
    {
        header.append("Connection:  keep-alive");
    }
    else
        header.append("Connection: Closed");
    header.append("\r\n");
}
void responseHeaderBuilder::setContentType(MIME_TYPE type, ENCODING e = ENCODING_UNDEFINED)
{
    std::string text = "Content-type: ";
   
    switch (type)
    {
    case 0:
        text.append("text/html");
        break;
    case 1:
        text.append("image/bmp");
        break;
    case 2:
        text.append("text/css");
        break;
    case 3:
        text.append("application/gzip");
        break;
    case 4:
        text.append("image/jpeg");
        break;
    case 5:
        text.append("text/javascript");
        break;
    case 6:
        text.append("application/json");
        break;
    case 7:
        text.append("video/mp4");
        break;
    case 8:
        text.append("image/png");
        break;
    case 9:
        
        text.append("application/pdf");
        break;
    case 10:
        text.append("image/svg+xml");
        break;
    case 11:
        text.append("text/plain");
        break;
    case 12:
        text.append("image/webp");
        break;
    case 13:
        text.append("application/xhtml+xml");
        break;
    case 14:
        text.append("application/xml");
        break;
    case 15:
        text.append("video/webm");
        break;
    case 16:
        break;
    case 17:
        text.append("application/octet-stream");
        break;
    case 18:
        text.append("video/mkv");
        break;
    default:
        text.append("text/plain");
        break;
    }
    if (e != ENCODING_UNDEFINED)
    {
        text.append(" ; charset=");
        if (e == ENCODING_UTF8)
        {
            text.append("UTF-8");
        }
    }
    text.append("\r\n");
    
    header.append(text);
}
void responseHeaderBuilder::set(std::string key, std::string value)
{
    header.append(key + ": " + value + "\r\n");
}
void responseHeaderBuilder::setHttpVersion(HTTP_VERSION version)
{
    std::string s;
    int val = header.find(" ");
    switch (version)
    {
    case 2:
        s.append("HTTP/2");
        header.replace(0, val, s);
        break;
    default:
        s.append("HTTP/1.1");

        header.replace(0, val, s);
    }
};
void responseHeaderBuilder::setHttpStatusCode(HTTP_STATUS_CODE s)
{
    int val = header.find(" ");
    int val2 = header.find(" ", val + 1);

    header.replace(val + 1, val2 - val - 1, std::to_string(s));
    switch (s)
    {
    case 200:

        header.replace(val2 + 1, header.find("\r") - val2 - 1, "OK");
        break;
    case 500:
        header.replace(val2 + 1, header.find("\r") - val2 - 1, "Internal Server Error");

        break;
    case 404:
        header.replace(val2 + 1, header.find("\r") - val2 - 1, "Not Found");

        break;
    case 302:
        header.replace(val2 + 1, header.find("\r") - val2 - 1, "Found");
        break;
    case 204:
        header.replace(val2 + 1, header.find("\r") - val2 - 1, "NO CONTENT");

        break;
    case 304:
        header.replace(val2 + 1, header.find("\r") - val2 - 1, "Not Modified");

        break;
    case 400:
        header.replace(val2 + 1, header.find("\r") - val2 - 1, "Bad Request");

        break;
    case 300:
        header.replace(val2 + 1, header.find("\r") - val2 - 1, "Multiple Choice");

        break;
    case 301:
        header.replace(val2 + 1, header.find("\r") - val2 - 1, "Moved Permenetly");

        break;
    case 303:
        header.replace(val2 + 1, header.find("\r") - val2 - 1, "See Other");

        break;
    case 307:
        header.replace(val2 + 1, header.find("\r") - val2 - 1, "Temporary Redirect");

        break;
    case 308:
        header.replace(val2 + 1, header.find("\r") - val2 - 1, "Permenent Redirect");
            break;
    }
};
void responseHeaderBuilder::setServer(std::string server)
{
    header.append("Server: " + server + "\r\n");
}
void responseHeaderBuilder::setContentEncoding(CONTENT_ENCODING E)
{
    header.append("Content-Encoding: ");
    switch (E)
    {
    case 0:
        header.append("gzip");
        break;
    case 1:
        header.append("compress");
        break;
    case 2:
        header.append("deflate");
        break;
    case 3:
        header.append("br");
        break;
    default:
        header.append("chunked");
    }
    header.append("\r\n");
}

void responseHeaderBuilder::setDate()
{
    currentDateTime d;
    header.append("Date: " + d.getResponseTime() + "\r\n");
}
responseHeaderBuilder::responseHeaderBuilder()
{
    header += "HTTP/1.1 200 OK\r\n";
}
responseHeaderBuilder::responseHeaderBuilder(HTTP_VERSION v, HTTP_STATUS_CODE status)
{
    switch (v)
    {
    case 2:
        header.append("HTTP/2 ");
        break;

    default:
        header.append("HTTP/1.1 ");
    }

    header.append(std::to_string(status));
    switch (status)
    {
    case 200:
        header.append(" OK");
        break;
    case 500:
        header.append(" Internal Server Error");
        break;
    case 404:
        header.append(" Not Found");
        break;
    case 302:
        header.append(" Found");
        break;
    case 204:
        header.append(" NO CONTENT");
        break;
    case 304:
        header.append(" Not Modified");
        break;
    case 400:
        header.append(" Bad Request");
        break;
    default:
        header.append(" Bad Request");
        break;
    }
    header.append("\r\n");
};
void responseHeaderBuilder::setCookie(std::string st)
{
     std::string cookie;
    cookie += "Set-Cookie: ";
    cookie += st;
    cookie += "\r\n";


    setString(cookie);

}
void responseHeaderBuilder::setCookie(std::string key, std::string value)
{
    std::string s;

    s += key;
    s += "=";
    s += value;

    setCookie(s);
};
void responseHeaderBuilder::setCookie(COOKIE cookie)
{
    if (cookie.name.empty())
        return;
    std::string s;
    s += cookie.name;
    s += "=";
    s += cookie.value;
    if (!cookie.expiry.empty())
    {
        s += "; Expires=";
        s += cookie.expiry;
    }
    if (cookie.secure == true)
    {
        s += "; Secure";
    }
    if (cookie.httpOnly == true)
    {
        s += "; HttpOnly";
    }
    if (cookie.maxAge != 0)
    {
        s += "; Max-Age=";
        s += to_string(cookie.maxAge);
    }
    if (!cookie.domain.empty())
    {
        s += "; Domain=";
        s += cookie.domain;
    }
    if (cookie.sameSite > 0 && cookie.sameSite < 4)
    {
        s += "; SameSite=";
        switch (cookie.sameSite)
        {
        case SAMESITE_TYPE_STRICT:
            s += "Strict";
            break;
        case SAMESITE_TYPE_LAX:
            s += "Lax";
            break;
        default:
            s += "None";
            break;
        }
    }
    if (!cookie.path.empty())
    {
        s += "; Path=";
        s += cookie.path;
    }
    setCookie(s);
}
#endif