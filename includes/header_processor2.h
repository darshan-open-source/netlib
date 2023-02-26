// bug free
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
#include <stringutil.h>
#include <standalone.h>
#include <get.h>
#include <post.h>
#include <multipart.h>
#include <debug.h>
#include <string>
using namespace std;

class header_processor2
{
public:
    header_processor2(std::shared_ptr<std::string> s) : buffer_array(s)
    {

        // initialize default value of headers
        header.method = HTTP_METHODS::HTTP_METHOD_UNDEFINED;
        header.payloadType = PAYLOAD_TYPE::PAYLOAD_TYPE_NO_PAYLOAD;
        header.httpVersion = HTTP_VERSION::HTTP_VERSION_UNDEFINED;
        header.connectionType = CONNECTION_TYPE::CONNECTION_TYPE_UNDEFINED;
    };
    HTTP_HEADER getheader() { return header; };

    void jointhread();
    
    void createthread();

    bool readded = 0;
    std::mutex m;
    unsigned long long readedx = 0;
    unsigned long long tobereadx = -1;
    int maxsize = MAX_HEADER_SIZE;
    bool error= false;
    void process_mainline(std::string);
    bool isPayloadProcessingError = false;

protected:
    std::shared_ptr<std::string> buffer_array;

    void process();

    void postparser();

    HTTP_HEADER header;

    void multipartparser();

    std::thread *threadh;
    string mainline;
};
