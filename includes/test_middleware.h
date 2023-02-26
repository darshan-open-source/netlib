#pragma once 
#include <middleware.h>

class DecodeData : public Middleware
{

public:
    DecodeData() : Middleware()
    {
    }

protected:
    RESPONSE __call__(httpRequest req, httpResponse res, DEFAULT_DATA not_usable)
    {

        HTTP_HEADER x = req.getFullHeader();
        if (x.payloadType == PAYLOAD_TYPE_GET_PAYLOAD || x.payloadType == PAYLOAD_TYPE_POST_PAYLOAD)
        {
                std::multimap<std::string,std::string> payload = std::get< std::multimap<std::string,std::string>>(x.payload);

            for (std::multimap<std::string, std::string>::iterator i = payload.begin(); i != payload.end(); i++)
            {
                i->second = decodeUrl(i->second);
                i->second = disableJs(i->second);
            }
        }
        else if (x.payloadType == PAYLOAD_TYPE_MULTIPART_PAYLOAD)
        {
                std::vector<MULTIPART_DATA> multipart_data = std::get<std::vector<MULTIPART_DATA>>(x.payload);

            for (size_t i = 0; i < multipart_data.size(); i++)
            {
                multipart_data.at(i).value = disableJs(multipart_data.at(i).value);
                multipart_data.at(i).name = disableJs(multipart_data.at(i).name);
            }
        }

        
        
        std::vector<URL_DATA> x2 = req.getFullUrlData();
        httpRequest reqx(x, x2);
        RESPONSE r = sendResponse(reqx, res, not_usable);

        return r;
    };
};