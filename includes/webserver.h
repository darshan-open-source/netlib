#include "settings.h"

#pragma once

#ifndef BLOCK_SIZE
#define BLOCK_SIZE 100000
#endif

#include "enums.h"
#include <iostream>

#ifdef USE_SSL
#include "SecureSocketServer.h"
#else

#include "SocketServer.h"

#endif

#include "header_processor2.h"
#include "Application.h"
#include "dynamicurl.h"
#include "debug.h"
#include "Router.h"

typedef std::function<RESPONSE(httpRequest, httpResponse, void *)> responseCallback;

typedef struct __DEFAULT_DATA {
    responseCallback callback;
    void *args;
    int type;
    int mlm;
} DEFAULT_DATA;

#include "default_middleware.h"

using namespace std;

class webserver : public darshan::network::SocketServer, public Router {
public:
    webserver(const char *host, int port) : darshan::network::SocketServer((char *) host, port) {

        middleware.push_back(new defaultMiddleware());
        middleware.push_back(new DecodeData());
    };

    void onClientConnected(shared_ptr <darshan::network::SocketConnection> s);

    void process(header_processor2 *, shared_ptr <darshan::network::SocketConnection>);

private:
    dynamicPath p;
    unsigned long long needRead = 0;

    void sendDirect(int, shared_ptr <darshan::network::SocketConnection> s);

    void sendRes(shared_ptr <darshan::network::SocketConnection> s, RESPONSE res);

    timeval vx = {4, 0};
    int timeout = 4000;

};

void webserver::onClientConnected(shared_ptr <darshan::network::SocketConnection> s) {


    shared_ptr <std::string> data = make_shared<std::string>();
    header_processor2 *hp = new header_processor2(data);
    s.get()->waitRead();
    setsockopt(s->getSocket().get().native_handle(), SOL_SOCKET, SO_RCVTIMEO, &vx, sizeof(vx));

    setsockopt(s->getSocket().get().native_handle(), SOL_TCP, TCP_USER_TIMEOUT, (char *) &timeout, sizeof(timeout));
    int i = 0;
    char *x = (char *) malloc(BLOCK_SIZE);

    long long readsize = 0, toberead = MAX_HEADER_SIZE;
    bool iserr = false;

    int v = BLOCK_SIZE;
    std::string d;

    // reading headers

    while (1) {
        size_t xlsx2;
        try {
            xlsx2 = s->readUntilCRLF(&d);
        }
        catch (std::exception &e) {

            cout << e.what();
            return;
        }

        string sxc = d.substr(0, xlsx2);

        if (xlsx2 == 2) {
            // lastline of headers
            data->erase(data->length(), 1);
            data->erase(data->length(), 1);
            readsize -= 2;
            if (d.length() != 2) {

                data.get()->append(d);

                readsize += d.length();
            }
            break;
        }

        hp->process_mainline(sxc);
        d.erase(0, xlsx2);
    }

    if (hp->getheader().method == HTTP_METHOD_GET) {
        needRead = 0;
        readsize = 0;
    } else if (hp->getheader().method == HTTP_METHOD_POST && !hp->getheader().contentLength.empty()) {
        if (isNumber(hp->getheader().contentLength) == true) {
            needRead = stoll(hp->getheader().contentLength);
        } else
            needRead = 0;
    }
    if (needRead == readsize && hp->getheader().method == HTTP_METHOD_POST &&
        hp->getheader().payloadType == PAYLOAD_TYPE_MULTIPART_PAYLOAD) {

        data->erase(data->length() - 4, 1);
        data->erase(data->length() - 3, 1);
    }

    hp->createthread();

    if (needRead != readsize) {

        while (1) {
            if (BLOCK_SIZE >= s->avalible()) {
                v = s->avalible();
            } else
                v = BLOCK_SIZE;

            if (v == 0 && needRead != readsize) {

                continue;
            }
            x = (char *) realloc(x, v + 1);

            size_t Z = s->readData(x, v);

            readsize += Z;

            if (readsize == needRead) {
                // cout  << "header is ended"<<endl;
                if (Z > 2) {
                    std::string st2(x, Z - 4);

                    if (hp->getheader().payloadType == PAYLOAD_TYPE_MULTIPART_PAYLOAD) {

                        string mns = st2;
                        try {

                            st2.append("\r\n");
                        }
                        catch (std::exception &e) {

                            cout << "readed text is " << x << " " << Z;
                            cout << "mns is " << mns << mns.length();
                            cout << "error occured at webserver " << st2 << endl;
                            return;
                        }
                    }
                    hp->m.lock();
                    data.get()->append(st2.c_str(), st2.length());
                    hp->m.unlock();
                }

                break;
            }

            hp->m.lock();
            data.get()->append(x, Z);
            hp->m.unlock();

            bzero(x, v);
        }
    }

    hp->readded = 1;
    hp->jointhread();

    process(hp, s);
};

void webserver::process(header_processor2 *hp, shared_ptr <darshan::network::SocketConnection> s) {


    std::string path = hp->getheader().path;

    int matched = -1;
    for (int i = 0; i < paths.size(); i++) {

        if (dynamicurls.at(i) != 1) {
            try {
                if (path.compare(paths.at(i)) == 0) {

                    matched = i;
#ifdef DEBUG
                    cout <<"[+] Processing path "<<path<<endl;
#endif

                    break;
                }
            }
            catch (std::exception &e) {
                cout << "error" << endl;
            }
        } else {

            if (p.isValidPath(path, durl.at(i))) {

                matched = i;
                break;
            }
        }
    }
    DEFAULT_DATA data;
    if (matched != -1) {
        if (callbacks.at(matched) != 0) {

            // processing middlewares
            data.callback = callbacks.at(matched);
            data.args = args.at(matched);
            data.type = 1;
            data.mlm = 0;
            dynamicPath p;
            p.isValidPath(path, durl.at(matched));
//            httpRequest r(hp->getheader(), getVariableValue(durl.at(matched), path)); // old dynamic url component
            httpRequest r(hp->getheader(), p.getPathData());   //updated dynamic url component

            httpResponse res(s, hp->getheader());
            RESPONSE rpx = middleware.at(0)->__call__(r, res, data);


            sendRes(s, rpx);
        } else {

#ifdef DEBUG
            cout << "Sending direct response for "<<path <<endl;
#endif
            sendDirect(matched, s);
        }
    } else {
#ifndef DEBUG
        cout << "Could not found path " << path << endl;
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

void webserver::sendDirect(int x, shared_ptr <darshan::network::SocketConnection> s) {
    HTTP_HEADER h;
    httpResponse r(s, h);
    MIME_TYPE m = filetypes.at(x);
    if (filetypes.at(x) == MIME_TYPE_SELF) {

        m = getFileTypeFromExtension(values.at(x));
    }

    char xbuffer[BLOCK_SIZE];

    FILE *f = fopen(values.at(x).c_str(), "r");
    if (f == 0) {
        r.setHttpStatusCode(HTTP_STATUS_CODE_Not_Found);
        r.sendHeader();
#ifdef DEBUG
        cout << "[+] file not found " << values.at(x) << endl;
#endif
        r.writer->writeString("<html><body><h1>Requested Url Not Found</h1></body></html>");
        return;
    } else {
        r.setHttpStatusCode(HTTP_STATUS_CODE_OK);
        r.setContentType(m);
        r.sendHeader();
    }
    while (!feof(f)) {
        int z = fread(xbuffer, 1, BLOCK_SIZE, f);
        r.writer->writeData(xbuffer, z);
    }

    fclose(f);
}

void webserver::sendRes(shared_ptr <darshan::network::SocketConnection> s, RESPONSE res) {
    s->writeString(res.header);

    if (res.type == 0) {
        s->writeString(res.val);
    } else {

        if (res.json == 0) {

            FILE *f = fopen(res.val.c_str(), "r");
            if (f == 0) {

                return;
            }
            char xbuffer[BLOCK_SIZE];
            while (!feof(f)) {
                int z = fread(xbuffer, 1, BLOCK_SIZE, f);
                s->writeData(xbuffer, z);
            }
            fclose(f);
        } else {
            inja::Environment env;

            s->writeString(env.render_file(res.val, res.json));
        }
    }
}
