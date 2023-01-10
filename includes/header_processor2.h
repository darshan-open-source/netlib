//bug free
#pragma once

#ifndef MAX_HEADER_SIZE
#define MAX_HEADER_SIZE 10000
#endif

#ifndef MAX_FILE_SIZE
#define MAX_FILE_SIZE 10485760
#endif

#ifndef MAX_PAYLOAD_SIZE
#define MAX_PAYLOAD_SIZE 10000
#endif

#include <vector>
#include <string>
#include <memory>
#include <thread>
#include <mutex>
#include <map>
#include "stringutil.h"
#include "standalone.h"
#include "get.h"
#include "post.h"
#include "multipart.h"
#include <debug.h>

using namespace std;

class header_processor2 {
public:
    header_processor2(std::shared_ptr <std::string> s) : buffer_array(s) {

        // initialize default value of headers
        header.method = HTTP_METHODS::HTTP_METHOD_UNDEFINED;
        header.payloadType = PAYLOAD_TYPE::PAYLOAD_TYPE_NO_PAYLOAD;
        header.version = HTTP_VERSION::HTTP_VERSION_UNDEFINED;
        header.connection_type = CONNECTION_TYPE::CONNECTION_TYPE_UNDEFINED;
    };

    HTTP_HEADER getheader() { return header; };

    void jointhread();

    void createthread();

    bool readded = 0;
    std::mutex m;
    unsigned long long readedx = 0;
    unsigned long long tobereadx = -1;
    int maxsize = MAX_HEADER_SIZE;

    void process_mainline(std::string);

protected:
    std::shared_ptr <std::string> buffer_array;

    void process();

    void postparser();

    HTTP_HEADER header;

    void multipartparser();

    std::thread *threadh;
    string mainline;
};

void header_processor2::process() {


    while (readded != 1 || buffer_array.get()->length() != 0) {
          if(buffer_array.get()->length()==0){
                  continue;
        }
        int pos = -1;

        m.lock();

        if (buffer_array->at(0) == '\r') {

            pos = 0;
        }

        if (pos != 0) {
            return;
        }

        // if pos == 0 then it is payload or it is a end of headers
        if (buffer_array->length() == 2) {
            //end of header
            readedx += 2;
            tobereadx = 0;
            buffer_array->erase(0, 2);
            m.unlock();
            return;
        }

        int x = -1;
        std::string boundry;
        std::vector <std::string> s = split(header.content_type, ';');
        if (s.size() == 0) return;

        string boundry2;
        std::vector <std::string> inner;
        try {
            if (stripString(toUpper(s.at(0))).compare("MULTIPART/FORM-DATA") == 0) {

                boundry2 = stripString(s.at(1));
                inner = split(boundry2, '=');
                if (toUpper(stripString(inner.at(0))).compare("BOUNDARY") == 0) {
                    boundry += stripString(inner.at(1));
                    x = 1;
                }
            }
        }
        catch (std::out_of_range &orx) {
            return;
        }
        if (stripString(toUpper(s.at(0))).compare("APPLICATION/X-WWW-FORM-URLENCODED") == 0) {
            x = 0;
        }

        if (x == 0) {
            // post data

            postparser();

            m.unlock();
            return;
        } else if (x == 1) {

            // multipart data

            string boundary1 = boundry;
            boundary1.insert(0, "\r\n--");
            boundary1.push_back('\r');
            boundary1.push_back('\n');

            header.payloadType = PAYLOAD_TYPE_MULTIPART_PAYLOAD;
            string rid = getRandomSessionId(10);

            while (readded != 1 || buffer_array->size() != 0) {

                int n_pos = buffer_array->find('\r', 1);

                string inner_bouandary = buffer_array->substr(0, n_pos + 2); // buffer_array->substr(0, n_pos + 2);

                if (inner_bouandary.compare(boundary1) == 0) {
                    buffer_array->erase(0, n_pos + 2);
                    if (buffer_array->size() == 0) {
                        m.unlock();
                        continue;
                    }

                    int rnrn_pos;
                    string tobefind = "\r\n\r\n";
                    auto it = std::search(buffer_array->begin(), buffer_array->end(), tobefind.data(),
                                          tobefind.data() + tobefind.length());

                    size_t x = std::distance(buffer_array->begin(), it);

                    if (x == buffer_array->size())
                        rnrn_pos = -1;
                    else
                        rnrn_pos = x;

                    if (rnrn_pos == -1) {

                        continue;
                    }
                    string vt = buffer_array->substr(0, rnrn_pos + 4);

                    MULTIPART_DATA d = multipartLineParser::parseLine(vt);

                    buffer_array->erase(0, 4 + rnrn_pos);

                    multipartWriter w(&d, rid);
                    m.unlock();

                    while (1) {

                        m.lock();

                        string s = *buffer_array;

                        size_t xl1 = s.find(boundary1);

                        FIND_BOUNDARY_RESULT x2;
                        x2.found = 0;

                        if (xl1 == std::string::npos) {

                            x2 = findBoundary(s, boundary1);
                        } else {
                            x2.found = true;
                            x2.full = true;
                        }

                        if (x2.found == true) {
                            int j;
                            if (x2.full != true) {
                                int yu = s.length();
                                s = buffer_array->substr(0, s.length() + x2.length);

                                if (s.length() < (yu + x2.length)) {

                                    m.unlock();
                                    continue;
                                }

                                j = s.find(boundary1);
                            } else {
                                j = s.find(boundary1);
                            }

                            if (j != -1) {
                                string j2 = buffer_array->substr(0, j);

                                w.writeData(j2);
                                buffer_array->erase(0, j);
                                m.unlock();
                                break;
                            } else {
                                w.writeData(s);
                                buffer_array->erase(0, s.length());
                            }
                        } else {

                            w.writeData(s);
                            buffer_array->erase(0, s.length());
                        }
                        m.unlock();
                    }
                    header.multipart_data.push_back(d);
                }
            }

            // multipart data ends
        }
        m.unlock();
    }
};

void header_processor2::createthread() {

    threadh = new std::thread(&header_processor2::process, this);
}

void header_processor2::jointhread() {
    threadh->join();
}

void header_processor2::postparser() {

    while (1) {
        if (buffer_array->length() == 0)
            continue;
        if (buffer_array->at(0) == '\r' || buffer_array->at(0) == '\n') {
            buffer_array->erase(buffer_array->begin());
            continue;
        } else
            break;
    }


    while (readded != 1);
    postParser p(*buffer_array.get());

    std::multimap <string, string> s = p.get();

    buffer_array->clear();
    header.payloadType = PAYLOAD_TYPE_URL_ENCODED_PAYLOAD;
    header.payload = p.get();
}

void header_processor2::process_mainline(std::string s) {

    s.erase(s.length() - 1);
    s.erase(s.length() - 1);
    int xmp = s.find(':', 0);
    if (xmp == std::string::npos) {
        // string is mainline or post payload

        int f1 = s.find(' ', 0);
        int f2 = s.find(' ', f1 + 1);
        if (f1 != -1 && f2 != -1) {
            //   cout  << "found";
            string method = s.substr(0, f1);
            string path = s.substr(f1, f2 - f1);
            string httpv = s.substr(f2, s.length() - f2);

            //   cout << method << " " << path << endl;

            if (toUpper(method).compare("GET") == 0) {
                // cout << "get";
                header.method = HTTP_METHOD_GET;
            } else if (toUpper(method).compare("POST") == 0)
                header.method = HTTP_METHOD_POST;
            else
                header.method = HTTP_METHOD_UNDEFINED;

            if (header.method == HTTP_METHOD_GET) {

                if (isGetQuery((char *) stripString(path).c_str())) {

                    header.path = getPathOfGetRequest((char *) stripString(path).c_str());
                    getParser g(stripString(path));
                    header.payload = g.get();
                    header.payloadType = PAYLOAD_TYPE_GET_PAYLOAD;
                } else {

                    header.path = stripString(path);
                }
            } else {

                header.path = stripString(path);
            }
            if (toUpper(stripString(httpv)).compare("HTTP/1.1") == 0) {
                header.version = HTTP_VERSION_HTTP1;
            } else if (toUpper(stripString(httpv)).compare("HTTP/2") == 0)
                header.version = HTTP_VERSION_HTTP2;
            else
                header.version = HTTP_VERSION_UNDEFINED;
        }
        s.clear();
    } else if (xmp >= 0) {
        // string is subline
        string key = s.substr(0, xmp);
        string value = s.substr(xmp + 1, s.length() - xmp);
        if (toUpper(stripString(key)).compare("HOST") == 0) {
            header.host = stripString(value);
        } else if (toUpper(stripString(key)).compare("CONNECTION") == 0) {
            if (toUpper(stripString(value)).compare("KEEP-ALIVE") == 0)
                header.connection_type = CONNECTION_TYPE_KEEP_ALIVE;
            else if (toUpper(stripString(value)).compare("CLOSE") == 0)
                header.connection_type = CONNECTION_TYPE::CONNECTION_TYPE_CLOSE;
            else
                header.connection_type = CONNECTION_TYPE::CONNECTION_TYPE_UNDEFINED;
        } else if (toUpper(stripString(key)).compare("SEC-FETCH-MODE") == 0) {
            header.sec_fetch_mode = stripString(value);
        } else if (toUpper(stripString(key)).compare("CONTENT-TYPE") == 0) {
            std::vector <std::string> stx = split(stripString(value), ';');
            string pt = stripString(stx.at(0));
            if (stx.size() != 0) {
                if (pt.compare("multipart/form-data") == 0)
                    header.payloadType = PAYLOAD_TYPE_MULTIPART_PAYLOAD;
                if (pt.compare("application/x-www-form-urlencoded") == 0)
                    header.payloadType = PAYLOAD_TYPE_URL_ENCODED_PAYLOAD;
            }
            header.content_type = stripString(value);
        } else if (toUpper(stripString(key)).compare("SEC-FETCH-SITE") == 0) {
            header.sec_fetch_site = stripString(value);
        } else if (toUpper(stripString(key)).compare("COOKIE") == 0) {
            header.cookieString = stripString(value);
        } else if (toUpper(stripString(key)).compare("SEC-FETCH-DEST") == 0) {
            header.sec_fetch_dest = stripString(value);
        } else if (toUpper(stripString(key)).compare("USER-AGENT") == 0) {
            header.user_agent = stripString(value);
        } else if (toUpper(stripString(key)).compare("UPGRADE-INSECURE-REQUESTS") == 0) {
            header.upgrade_insecure_request = stripString(value);
        } else if (toUpper(stripString(key)).compare("ACCEPT") == 0) {
            header.accept = split(stripString(value), ',');
        } else if (toUpper(stripString(key)).compare("CONTENT-LENGTH") == 0) {
            //    stoll
            header.contentLength = stripString(value);

            //   cout << " length is ser   "<<header.contentLength <<"   "<<stoll(stripString(value))<<endl;
        } else if (toUpper(stripString(key)).compare("ACCEPT-ENCODING") == 0) {
            vector <CONTENT_ENCODING> v;
            vector <string> sp = split(stripString(value), ',');
            for (int i = 0; i < sp.size(); i++) {
                if (toUpper(stripString(sp.at(i))).compare("DEFLATE") == 0) {
                    v.push_back(CONTENT_ENCODING_DEFLATE);
                }
                if (toUpper(stripString(sp.at(i))).compare("BR") == 0) {
                    v.push_back(CONTENT_ENCODING_BR);
                }
                if (toUpper(stripString(sp.at(i))).compare("CHUNKED") == 0) {
                    v.push_back(CONTENT_ENCODING_CHUNKED);
                }
                if (toUpper(stripString(sp.at(i))).compare("GZIP") == 0) {
                    v.push_back(CONTENT_ENCODING_GZIP);
                }

                header.accept_encoding = v;
            }
        } else if (toUpper(stripString(key)).compare("ACCEPT-LANGUAGE") == 0) {

            header.accept_language = split(stripString(value), ',');
        } else if (toUpper(stripString(key)).compare("CACHE-CONTROL") == 0) {

            header.cache_control = stripString(value);
        } else {
        }
    }
}
