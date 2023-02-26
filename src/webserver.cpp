#include <webserver.h>
void webserver::onClientConnected(shared_ptr<darshan::network::SocketConnection> s)
{

    shared_ptr<std::string> data = make_shared<std::string>();
    header_processor2 *hp = new header_processor2(data);
    s.get()->waitRead();
    // setsockopt(s->getSocket().get().native_handle(), SOL_SOCKET, SO_RCVTIMEO, &vx, sizeof(vx));

    // setsockopt(s->getSocket().get().native_handle(), SOL_TCP, TCP_USER_TIMEOUT, (char *)&timeout, sizeof(timeout));

    long long readsize = 0;
    bool iserr = false;

    size_t v = BLOCK_SIZE;
    std::string d;

    // reading headers
    short readsizemx = 0;
    while(readsizemx <= MAX_HEADER_LEN)
    {
        size_t xlsx2;
        try
        {
            xlsx2 = s->readUntilCRLF(&d);
            readsizemx += d.length();
        }
        catch (std::exception &e)
        {

            cout << e.what();
            return;
        }

        string sxc = d.substr(0, xlsx2);

        if (xlsx2 == 2)
        {
            // lastline of headers

            data->erase(data->length(), 1);
            data->erase(data->length(), 1);

            readsize -= 2;
            if (d.length() != 2)
            {

                data.get()->append(d);

                readsize += d.length();
            }
            break;
        }

        hp->process_mainline(sxc);

        d.erase(0, xlsx2);
    }

    if (readsizemx > MAX_HEADER_LEN)
    {

        s->writeString("200 OK\r\n\r\n<html><body>sorry</body></html>");
        s->close();
        return;
    }
    HTTP_METHODS req_m  = hp->getheader().method;
    bool isPayloadHTTPheader= false;
    if (req_m == HTTP_METHOD_GET || req_m == HTTP_METHOD_DELETE || req_m == HTTP_METHOD_HEAD || req_m == HTTP_METHOD_OPTIONS)
    {
        needRead = 0;
        readsize = 0;

    }
    else if ((req_m == HTTP_METHOD_POST || req_m == HTTP_METHOD_PUT || req_m == HTTP_METHOD_PATCH) && !hp->getheader().contentLength.empty())
    {
        isPayloadHTTPheader = true;
        if (isNumber(hp->getheader().contentLength) == true)
        {
            needRead = stoll(hp->getheader().contentLength);
        }
        else
            needRead = 0;
    }
    if (needRead == readsize && hp->getheader().method == HTTP_METHOD_POST &&
        hp->getheader().payloadType == PAYLOAD_TYPE_MULTIPART_PAYLOAD)
    {
        
        data->erase(data->length() - 4, 1);
        data->erase(data->length() - 3, 1);
    }

    if (needRead != readsize || isPayloadHTTPheader ==  true)
    {
        hp->createthread();
        size_t payloadSizeCounter = 0;

        while (payloadSizeCounter <= MAX_PAYLOAD_SIZE)
        {
            char *x = (char *)calloc(BLOCK_SIZE + 1, 1);

            x[BLOCK_SIZE] = '\0';

            size_t av = s->avaliable();
            if (BLOCK_SIZE > av)
            {
                v = av;
            }
            else
                v = BLOCK_SIZE;

            if (v == 0 && needRead != readsize)
            {
                free(x);
                continue;
            }

            size_t Z = s->readData(x, v);
            // std::cout << "\nreaded is " << x;
            payloadSizeCounter += Z;
            readsize += Z;

            if (readsize == needRead)
            {
                if (Z > 2)
                {

                    // std::cout << "\nst2 is " <<st2<<"\n";

                    if (hp->getheader().payloadType == PAYLOAD_TYPE_MULTIPART_PAYLOAD)
                    {
                        std::string st2(x, Z - 4);
                        st2.append("\r\n");
                        hp->m.lock();
                        data.get()->append(st2.c_str(), st2.length());
                        hp->m.unlock();
                    }
                    else
                    {
                        hp->m.lock();
                        data.get()->append(x, Z);
                        hp->m.unlock();
                    }
                }
                free(x);
                break;
            }
            hp->m.lock();

            data.get()->append(x, Z);
            hp->m.unlock();

            free(x);
        }
        if (payloadSizeCounter > MAX_PAYLOAD_SIZE)
        {
            removeAllFilesFromMultipart(std::get<std::vector<MULTIPART_DATA>>(hp->getheader().payload));

            hp->readded = true;

            hp->error = true;
            data->clear();
            hp->jointhread();

            int x = s->writeString("413 Request Entity Too Large\r\n\r\n");

            s->close();
            return;
        }
        hp->readded = 1;
        hp->jointhread();
    }

    process(hp, s);
};

void webserver::process(header_processor2 *hp, shared_ptr<darshan::network::SocketConnection> s)
{

    std::string path = hp->getheader().path;

    int matched = -1;
    for (int i = 0; i < paths.size(); i++)
    {

        if (dynamicurls.at(i) != 1)
        {

            if (path.compare(paths.at(i)) == 0)
            {

                matched = i;
#ifdef DEBUG
                cout << "[+] Processing path " << path << endl;
#endif

                break;
            }
        }
        else
        {

            if (p.isValidPath(path, durl.at(i)))
            {

                matched = i;
                break;
            }
        }
    }
    DEFAULT_DATA data;
    if (matched != -1)
    {
        if (callbacks.at(matched) != 0)
        {

            // processing middlewares
            data.callback = callbacks.at(matched);
            data.args = args.at(matched);
            data.type = 1;
            data.mlm = 0;
            dynamicPath p;
            p.isValidPath(path, durl.at(matched));
            //            httpRequest r(hp->getheader(), getVariableValue(durl.at(matched), path)); // old dynamic url component
            httpRequest r(hp->getheader(), p.getPathData()); // updated dynamic url component

            httpResponse res(s, hp->getheader());
            RESPONSE rpx = middleware.at(0)->__call__(r, res, data);

            sendRes(s, rpx);
        }
        else
        {

#ifdef DEBUG
            cout << "[+] Sending direct response for " << path << endl;
#endif
            sendDirect(matched, s);
        }
    }
    else
    {
#ifdef DEBUG
        cout << "[+] Could not found path " << path << endl;
#endif
        data.callback = df;
        data.args = dargs;
        data.type = 1;
        data.mlm = 0;
        RESPONSE rpx = middleware.at(0)->__call__(httpRequest(hp->getheader(), std::vector<URL_DATA>()),
                                                  httpResponse(s, hp->getheader()), data);

        sendRes(s, rpx);
    }
}

void webserver::sendDirect(int x, shared_ptr<darshan::network::SocketConnection> s)
{
    HTTP_HEADER h;
    httpResponse r(s, h);
    MIME_TYPE m = filetypes.at(x);
    if (filetypes.at(x) == MIME_TYPE_SELF)
    {

        m = getFileTypeFromExtension(values.at(x));
    }

    char xbuffer[BLOCK_SIZE];

    FILE *f = fopen(values.at(x).c_str(), "r");
    if (f == 0)
    {
        r.setHttpStatusCode(HTTP_STATUS_CODE_Internal_Server_Error);
        r.sendHeader();
#ifdef DEBUG
        cout << "[+] file not found " << values.at(x) << endl;
#endif

        r.writer->writeString("<html><body><h1>Internal server error</h1></body></html>");
        return;
    }
    else
    {
        r.setHttpStatusCode(HTTP_STATUS_CODE_OK);
        r.setContentType(m);
        r.sendHeader();
    }
    fclose(f);
    writeFileToSocket(s, values.at(x));
}

void webserver::sendRes(shared_ptr<darshan::network::SocketConnection> s, RESPONSE res)
{
    s->writeString(res.header);

    if (res.type == 0)
    {
        s->writeString(res.val);
    }
    else
    {

        if (res.json == 0)
        {
            writeFileToSocket(s, res.val);
        }
        else
        {
            inja::Environment env;

            s->writeString(env.render_file(res.val, res.json));
        }
    }
}
