#include "middleware.h"

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
        std::vector<URL_DATA> x2 = req.getFullUrlData();

        for (std::multimap<std::string, std::string>::iterator i = x.payload.begin(); i != x.payload.end(); i++)
        {
            i->second = decodeUrl(i->second);
            i->second = disableJs(i->second);
           

        }
       if(!x.multipart_data.empty())
       {
           for (size_t i = 0; i < x.multipart_data.size(); i++)
           {
              x.multipart_data.at(i).value = disableJs(x.multipart_data.at(i).value);
              x.multipart_data.at(i).name = disableJs(x.multipart_data.at(i).name);

           }
           
       }
        httpRequest reqx(x, x2);
        RESPONSE r = sendResponse(reqx, res, not_usable);

        return r;
    };
};