#ifndef JWTX_INCLUDED
#define JWTX_INCLUDED
#include <jwt-cpp/traits/nlohmann-json/traits.h>
#include <jwt-cpp/jwt.h>
#include <nlohmann/json.hpp>
#include <sstream>
#include <strstream>
#include <iostream>
using namespace std;

#ifndef JWT_SECRET
#define JWT_SECRET "pKZEZl7EXBXRs+XZcg4BAUP0BBiRItECFO+/LNrx1pABo64mDUaRUPdgtgOYjYI0hkZGYZG1QbgUAd7w4nbAEh7SbbnLgM5S1g7Aaudo8a10cG7snrel4sOvuTrfQwUJ35qULZKc2F9j80iwBqqUZulfVs3j1+/U9yz+VJKk4+/6yfEsNXYxLrw/xJJDfQxESfpOvgdR0rOdrufJ6Pb"
#endif

#ifndef JWT_ISSUER
#define JWT_ISSUER "darshan"
#endif

#ifndef JWT_ALGORITHM
#define JWT_ALGORITHM jwt::algorithm::hs512
#endif

#ifndef JWT_TYPE
#define JWT_TYPE "JWT"
#endif

class jwtx
{
public:
    jwtx(){};
    bool verify(std::string);
    std::string create(nlohmann::json = NULL);
};
bool jwtx::verify(std::string token)
{
    auto decoded = jwt::decode(token);

    for (auto &e : decoded.get_payload_claims())
    {
        cout << e.first << "=" << e.second << endl;
    }
    auto verifier = jwt::verify().with_issuer(JWT_ISSUER).allow_algorithm(JWT_ALGORITHM{JWT_SECRET});
    try
    {
        verifier.verify(decoded);
    }
    catch (jwt::error::signature_verification_exception &e)
    {
        return false;
    }
    return true;
}
using namespace nlohmann;
std::string jwtx::create(nlohmann::json j)
{
    using ts = jwt::traits::nlohmann_json;
    using claim = jwt::basic_claim<ts>;
    auto token = jwt::create<jwt::traits::nlohmann_json>().set_type(JWT_TYPE).set_issuer(JWT_ISSUER);

    for (json::iterator i = j.begin(); i != j.end(); i++)
    {
        if (i.value().is_string())
        {

           token.set_payload_claim(i.key(), i.value());
        }
       else if (i.value().is_array())
        {
            std::vector<std::string> vc;
            for (auto &e : i.value())
            {
              
                vc.push_back(e.dump());
                
            }
            token.set_payload_claim(i.key(), {vc.begin(), vc.end()});
        }
        else if (i.value().is_object())
        {
            claim raw_json;
            std::istringstream stream(i.value().dump());
            stream >> raw_json;
            token.set_payload_claim(i.key(), raw_json);
        }
        else
        {
           token.set_payload_claim(i.key(), i.value());
        }
    }

    
    return token.sign(JWT_ALGORITHM{JWT_SECRET});
}

#endif