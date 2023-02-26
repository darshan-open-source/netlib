#include <response_header.h>

void responseHeaderBuilder::setContentLength(size_t length)
{
    resp_object.contentLength = length;
}

std::string responseHeaderBuilder::getHeader()
{
    switch (resp_object.version)
    {
    case 2:
        header.append("HTTP/2 ");
        break;

    default:
        header.append("HTTP/1.1 ");
    }

    header.append(std::to_string(resp_object.status_code));
    switch (resp_object.status_code)
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

    if (!resp_object.server.empty())
    {
        header += "Server: ";
        header += resp_object.server;
        header += "\r\n";
    }
    if (!resp_object.date.empty())
    {
        header += "Date: ";
        header += resp_object.date;
        header += "\r\n";
    }
    if (resp_object.contentLength > 0)
    {
        header += "Content-Length: ";
        header += std::to_string(resp_object.contentLength);
        header += "\r\n";
    }
    if (resp_object.contentEncoding != CONTENT_ENCODING_UNDEFINED)
    {
        header.append("Content-Encoding: ");
        switch (resp_object.contentEncoding)
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
    
    if (resp_object.contentType != MIME_TYPE_UNDEFINED)
    {
        header += "Content-type: ";

        switch (resp_object.contentType)
        {
        case 0:
            header.append("text/html");
            break;
        case 1:
            header.append("image/bmp");
            break;
        case 2:
            header.append("text/css");
            break;
        case 3:
            header.append("application/gzip");
            break;
        case 4:
            header.append("image/jpeg");
            break;
        case 5:
            header.append("text/javascript");
            break;
        case 6:
            header.append("application/json");
            break;
        case 7:
            header.append("video/mp4");
            break;
        case 8:
            header.append("image/png");
            break;
        case 9:

            header.append("application/pdf");
            break;
        case 10:
            header.append("image/svg+xml");
            break;
        case 11:
            header.append("text/plain");
            break;
        case 12:
            header.append("image/webp");
            break;
        case 13:
            header.append("application/xhtml+xml");
            break;
        case 14:
            header.append("application/xml");
            break;
        case 15:
            header.append("video/webm");
            break;
        case 16:
            break;
        case 17:
            header.append("application/octet-stream");
            break;
        case 18:
            header.append("video/mkv");
            break;
        default:
            header.append("text/plain");
            break;
        }
        if (resp_object.encoding != ENCODING_UNDEFINED)
        {
            header.append(" ; charset=");
            if (resp_object.encoding == ENCODING_UTF8)
            {
                header.append("UTF-8");
            }
        }
        header.append("\r\n");
        
    }
    for (size_t i = 0; i < resp_object.cookies.size(); i++)
    { 
        COOKIE cookie = resp_object.cookies[i];
        if (cookie.name.empty())
            continue;

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
            s += std::to_string(cookie.maxAge);
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
        header+="Set-Cookie: ";
        header+=s;
        header+="\r\n";
    }
    if (resp_object.connection_type != CONNECTION_TYPE_UNDEFINED)
    {
        if (resp_object.connection_type == CONNECTION_TYPE_KEEP_ALIVE)
        {
            header.append("Connection: keep-alive");
        }
        else
            header.append("Connection: Closed");
        header.append("\r\n");
    }
    for (stringMultimapIterator i = resp_object.others.begin(); i != resp_object.others.end(); i++)
    {
        header += i->first;
        header += ": ";
        header += i->second;
        header += "\r\n";
    }
    header+="\r\n";

    return header;
}

void responseHeaderBuilder::setConnectionType(CONNECTION_TYPE t)
{
    resp_object.connection_type = t;
}
void responseHeaderBuilder::setContentType(MIME_TYPE type, ENCODING e)
{
    resp_object.contentType = type;
    resp_object.encoding = e;
}
void responseHeaderBuilder::set(std::string key, std::string value)
{
    for (stringMultimapIterator i = resp_object.others.begin(); i != resp_object.others.end(); i++)
    {
        if (i->first.compare(key) == 0)
        {
            i->second = value;
            return;
        }
    }
    resp_object.others.insert({key, value});
}
void responseHeaderBuilder::setHttpVersion(HTTP_VERSION version)
{
    resp_object.version = version;
};
void responseHeaderBuilder::setHttpStatusCode(HTTP_STATUS_CODE s)
{

    resp_object.status_code = s;
};
void responseHeaderBuilder::setServer(std::string server)
{
    resp_object.server = server;
}
void responseHeaderBuilder::setContentEncoding(CONTENT_ENCODING E)
{
    resp_object.contentEncoding = E;
}

void responseHeaderBuilder::setDate()
{
    currentDateTime d;

    resp_object.date = d.getResponseTime();

}
responseHeaderBuilder::responseHeaderBuilder()
{
       resp_object.connection_type = CONNECTION_TYPE_UNDEFINED;
     resp_object.contentEncoding = CONTENT_ENCODING_UNDEFINED;
     resp_object.contentType = MIME_TYPE_UNDEFINED;
     resp_object.encoding= ENCODING_UNDEFINED;
    resp_object.status_code =HTTP_STATUS_CODE_OK;
         resp_object.contentLength=0;

    resp_object.version = HTTP_VERSION_HTTP1;
}
responseHeaderBuilder::responseHeaderBuilder(HTTP_VERSION v, HTTP_STATUS_CODE status)
{
     resp_object.connection_type = CONNECTION_TYPE_UNDEFINED;
     resp_object.contentEncoding = CONTENT_ENCODING_UNDEFINED;
     resp_object.contentType = MIME_TYPE_UNDEFINED;
     resp_object.encoding= ENCODING_UNDEFINED;
     resp_object.contentLength=0;
    resp_object.version = v;
    resp_object.status_code = status;
};

void responseHeaderBuilder::setCookie(std::string key, std::string value)
{

    COOKIE c;
    c.name = key;
    c.value = value;
    c.maxAge=0;
    setCookie(c);
};
void responseHeaderBuilder::setCookie(COOKIE cookie)
{
    for (size_t i = 0; i < resp_object.cookies.size(); i++)
    {
        if (resp_object.cookies[i].name.compare(cookie.name) == 0)
        {
            resp_object.cookies[i] = cookie;
            return;
        }
    }
    resp_object.cookies.push_back(cookie);
}