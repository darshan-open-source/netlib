#include <header_processor2.h>

void header_processor2::process()
{

    while (readded != 1 || buffer_array.get()->length() != 0)
    {

        if (buffer_array.get()->length() == 0)
        {
            continue;
        }

        int pos = -1;

        m.lock();

        if (buffer_array->at(0) == '\r')
            pos = 0;

        if (pos != 0)
        {
            // invalid http header
            isPayloadProcessingError = true;
            m.unlock();
            return;
        }

        // if pos == 0 then it is payload or it is a end of headers
        if (buffer_array->length() == 2 && readded == 1)
        {
            // end of header
            readedx += 2;
            tobereadx = 0;

            buffer_array->erase(0, 2);
            m.unlock();
            return;
        }

        if (header.payloadType == PAYLOAD_TYPE_MULTIPART_PAYLOAD)
        {

            std::string boundry;
            std::vector<std::string> s = split(header.contentType, ';');
            if (s.size() == 0)
            {
                // console::log("end of headers error");

                m.unlock();
                isPayloadProcessingError = true;
                return;
            }
            string boundry2;
            std::vector<std::string> inner;
            if (s.size() != 2)
            {
                isPayloadProcessingError = true;
                m.unlock();
                // console::log("end of headers 2");

                return;
            }
            boundry2 = stripString(s.at(1));
            inner = split(boundry2, '=');
            if (toUpper(stripString(inner.at(0))).compare("BOUNDARY") == 0)
            {
                if (inner.size() != 2)
                {
                    isPayloadProcessingError = true;
                    m.unlock();
                    // console::log("end of headers 3");

                    return;
                }

                boundry += stripString(inner.at(1));
            }
            // multipart data

            header.payload = std::vector<MULTIPART_DATA>();

            string boundary1 = boundry;
            boundary1.insert(0, "\r\n--");
            boundary1.push_back('\r');
            boundary1.push_back('\n');

            string rid = getRandomSessionId(10);

            m.unlock();

            while (readded != 1 || buffer_array->size() != 0)
            {
                ;
                m.lock();

                int n_pos = buffer_array->find('\r', 1);
                if (n_pos == std::string::npos)
                {
                    // boundary is not found

                    continue;
                }

                string inner_bouandary = buffer_array->substr(0, n_pos + 2);
                if (inner_bouandary.compare(boundary1) == 0 && readded == 1 && buffer_array->length() == boundary1.length())
                {
                    buffer_array->clear();
                    std::vector<MULTIPART_DATA> datas = std::get<std::vector<MULTIPART_DATA>>(header.payload);

                    return;
                }
                if (inner_bouandary.compare(boundary1) == 0)
                {
                    int rnrn_pos;
                    string tobefind = "\r\n\r\n";
                    auto it = std::search((buffer_array->begin() + n_pos + 2), buffer_array->end(), tobefind.data(),
                                          tobefind.data() + tobefind.length());
                    if (it == buffer_array->end())
                    {

                        m.unlock();
                        continue;
                    }
                    size_t x = std::distance((buffer_array->begin() + n_pos + 2), it);

                    string vt = buffer_array->substr(n_pos + 2, x + 4);

                    MULTIPART_DATA d = multipartLineParser::parseLine(vt);

                    buffer_array->erase(0, 4 + x + 2 + n_pos);

                    multipartWriter w(&d, rid);
                    // std::get<std::vector<MULTIPART_DATA>>(header.payload).push_back(d);

                    m.unlock();

                    while (1 && error != true)
                    {

                        m.lock();

                        string s = *buffer_array;

                        size_t xl1 = s.find(boundary1);

                        FIND_BOUNDARY_RESULT x2;
                        x2.found = 0;

                        if (xl1 == std::string::npos)
                        {

                            x2 = findBoundary(s, boundary1);
                        }
                        else
                        {
                            x2.found = true;
                            x2.full = true;
                            x2.start = xl1;
                        }

                        if (x2.found == true)
                        {
                            int j;
                            if (x2.full != true)
                            {
                                // boundary is found but full boundary is not found

                                string v = buffer_array->substr(0, x2.start);

                                w.writeData(v);
                                buffer_array->erase(buffer_array->begin(), buffer_array->begin() + x2.start);
                                m.unlock();
                                continue;
                            }
                            else
                            {
                                // full boundary is found
                                j = s.find(boundary1);
                                string j2 = buffer_array->substr(0, j);
                                w.writeData(j2);
                                buffer_array->erase(0, j);
                                m.unlock();
                                break;
                            }
                        }
                        else
                        {

                            w.writeData(s);
                            buffer_array->erase(0, s.length());
                        }
                        m.unlock();
                    }
                    std::get<std::vector<MULTIPART_DATA>>(header.payload).push_back(d);
                }
                else
                {
                    // boundary is found but not matched
                    isPayloadProcessingError = true;
                }
            }
        }

        else if (header.payloadType == PAYLOAD_TYPE_POST_PAYLOAD)
        {

            // post data x-www-form-urlencoded data is arrived

            m.unlock();
            postparser();
            return;
        }
        else
        {
            // for any type of data like JSON ,   XML

            buffer_array->erase(0, 2);
            m.unlock();
            header.payloadType = PAYLOAD_TYPE_OTHER;
            std::string s = uploadedFilesDirectory;
            while (1)
            {

                s += getRandomSessionId(20);
                if (!std::filesystem::exists(s))
                    break;
            }

            FILE *f = fopen(s.c_str(), "wb");
            header.payload = s;
            if (f == 0)
                return;
            while (readded != 1 || buffer_array->length() != 0)
            {
                m.lock();
                fwrite(buffer_array->data(), 1, buffer_array->length(), f);
                buffer_array->clear();
                m.unlock();
            }
            fclose(f);
            return;
        }


        m.unlock();
    }
};

void header_processor2::createthread()
{
    threadh = new std::thread(&header_processor2::process, this);
}

void header_processor2::jointhread()
{
    threadh->join();
}

void header_processor2::postparser()
{

    while (readded != 1)
        ;
    m.lock();
    while (1)
    {
        if (buffer_array->length() == 0)
            continue;
        if (buffer_array->at(0) == '\r' || buffer_array->at(0) == '\n')
        {
            buffer_array->erase(buffer_array->begin());
            continue;
        }
        else
            break;
    }

    // console::log(*buffer_array.get());
    postParser p(*buffer_array.get());

    buffer_array->clear();
    header.payloadType = PAYLOAD_TYPE_POST_PAYLOAD;
    header.payload = p.get();
    m.unlock();
}

void header_processor2::process_mainline(std::string s)
{

    s.erase(s.length() - 1);
    s.erase(s.length() - 1);
    int xmp = s.find(':', 0);
    if (xmp == std::string::npos)
    {
        // string is mainline or post payload

        int f1 = s.find(' ', 0);
        int f2 = s.find(' ', f1 + 1);
        if (f1 != -1 && f2 != -1)
        {
            string method = s.substr(0, f1);
            string path = s.substr(f1, f2 - f1);
            string httpv = s.substr(f2, s.length() - f2);

            if (toUpper(method).compare("GET") == 0)

                header.method = HTTP_METHOD_GET;

            else if (toUpper(method).compare("POST") == 0)
                header.method = HTTP_METHOD_POST;
            else if (toUpper(method).compare("PUT") == 0)
                header.method = HTTP_METHOD_PUT;

            else if (toUpper(method).compare("HEAD") == 0)
                header.method = HTTP_METHOD_HEAD;

            else if (toUpper(method).compare("OPTIONS") == 0)
                header.method = HTTP_METHOD_OPTIONS;

            else if (toUpper(method).compare("PATCH") == 0)
                header.method = HTTP_METHOD_PATCH;

            else if (toUpper(method).compare("DELETE") == 0)
                header.method = HTTP_METHOD_DELETE;

            else
                header.method = HTTP_METHOD_UNDEFINED;

            if (header.method == HTTP_METHOD_GET)
            {

                if (isGetQuery((char *)stripString(path).c_str()))
                {

                    header.path = getPathOfGetRequest((char *)stripString(path).c_str());
                    getParser g(stripString(path));
                    header.payload = g.get();
                    header.payloadType = PAYLOAD_TYPE_GET_PAYLOAD;
                }
                else
                {

                    header.path = stripString(path);
                }
            }
            else
            {

                header.path = stripString(path);
            }
            if (toUpper(stripString(httpv)).compare("HTTP/1.1") == 0)
            {
                header.httpVersion = HTTP_VERSION_HTTP1;
            }
            else if (toUpper(stripString(httpv)).compare("HTTP/2") == 0)
                header.httpVersion = HTTP_VERSION_HTTP2;
            else
                header.httpVersion = HTTP_VERSION_UNDEFINED;
        }
        s.clear();
    }
    else if (xmp >= 0)
    {
        // string is subline
        string key = s.substr(0, xmp);
        string value = s.substr(xmp + 1, s.length() - xmp);
        if (toUpper(stripString(key)).compare("HOST") == 0)
        {
            header.host = stripString(value);
        }
        else if (toUpper(stripString(key)).compare("CONNECTION") == 0)
        {
            if (toUpper(stripString(value)).compare("KEEP-ALIVE") == 0)
                header.connectionType = CONNECTION_TYPE_KEEP_ALIVE;
            else if (toUpper(stripString(value)).compare("CLOSE") == 0)
                header.contentType = CONNECTION_TYPE::CONNECTION_TYPE_CLOSE;
            else
                header.contentType = CONNECTION_TYPE::CONNECTION_TYPE_UNDEFINED;
        }
        else if (toUpper(stripString(key)).compare("SEC-FETCH-MODE") == 0)
        {
            header.secFetchMode = stripString(value);
        }
        else if (toUpper(stripString(key)).compare("CONTENT-TYPE") == 0)
        {
            std::vector<std::string> stx = split(stripString(value), ';');
            if (header.method == HTTP_METHOD_POST)
            {
                string pt = stripString(stx.at(0));
                if (pt.compare("multipart/form-data") == 0)
                    header.payloadType = PAYLOAD_TYPE_MULTIPART_PAYLOAD;
                else if (pt.compare("application/x-www-form-urlencoded") == 0)
                    header.payloadType = PAYLOAD_TYPE_POST_PAYLOAD;
                else
                    header.payloadType = PAYLOAD_TYPE_OTHER;
            }
            header.contentType = stripString(value);
        }
        else if (toUpper(stripString(key)).compare("SEC-FETCH-SITE") == 0)
        {
            header.secFetchSite = stripString(value);
        }
        else if (toUpper(stripString(key)).compare("COOKIE") == 0)
        {
            header.cookieString = stripString(value);
        }
        else if (toUpper(stripString(key)).compare("SEC-FETCH-DEST") == 0)
        {
            header.secFetchDest = stripString(value);
        }
        else if (toUpper(stripString(key)).compare("USER-AGENT") == 0)
        {
            header.userAgent = stripString(value);
        }
        else if (toUpper(stripString(key)).compare("UPGRADE-INSECURE-REQUESTS") == 0)
        {
            header.upgradeInsecureRequest = stripString(value);
        }
        else if (toUpper(stripString(key)).compare("ACCEPT") == 0)
        {
            header.accept = split(stripString(value), ',');
        }
        else if (toUpper(stripString(key)).compare("CONTENT-LENGTH") == 0)
        {
            //    stoll
            header.contentLength = stripString(value);

            //   cout << " length is ser   "<<header.contentLength <<"   "<<stoll(stripString(value))<<endl;
        }
        else if (toUpper(stripString(key)).compare("ACCEPT-ENCODING") == 0)
        {
            vector<CONTENT_ENCODING> v;
            vector<string> sp = split(stripString(value), ',');
            for (int i = 0; i < sp.size(); i++)
            {
                if (toUpper(stripString(sp.at(i))).compare("DEFLATE") == 0)
                {
                    v.push_back(CONTENT_ENCODING_DEFLATE);
                }
                if (toUpper(stripString(sp.at(i))).compare("BR") == 0)
                {
                    v.push_back(CONTENT_ENCODING_BR);
                }
                if (toUpper(stripString(sp.at(i))).compare("CHUNKED") == 0)
                {
                    v.push_back(CONTENT_ENCODING_CHUNKED);
                }
                if (toUpper(stripString(sp.at(i))).compare("GZIP") == 0)
                {
                    v.push_back(CONTENT_ENCODING_GZIP);
                }

                header.acceptEncoding = v;
            }
        }
        else if (toUpper(stripString(key)).compare("ACCEPT-LANGUAGE") == 0)
        {

            header.acceptLanguage = split(stripString(value), ',');
        }
        else if (toUpper(stripString(key)).compare("CACHE-CONTROL") == 0)
        {

            header.cacheControl = stripString(value);
        }
        else
        {
            header.anotherHeaders.insert(std::make_pair<std::string, std::string>(stripString(key), stripString(value)));
        }
    }
}
