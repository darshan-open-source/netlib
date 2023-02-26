//usable via Router

#pragma once
#include <filesystem>
#include <plateform/os.h>
#include <string>
#include <httpResponse.h>

#include <vector>

class publicDirectory
{
public:
    publicDirectory(std::string, httpResponse,std::string);
    RESPONSE getResponse();
private:
    std::vector<string> fname;
    std::vector<string> ftime;
    std::vector<string> fdate;
    inja::json j;

    std::vector<string> fsize;

    string getfilesize(uintmax_t y)
    {
        string s;
        int i{};

        double mantissa = y;
        for (; mantissa >= 1024.; ++i)
        {
            mantissa /= 1024.;
        }
        mantissa = std::ceil(mantissa * 10.) / 10.;
        s.append(to_string((int)mantissa));

        s += "BKMGTPE"[i];
        return s;
    }
};
