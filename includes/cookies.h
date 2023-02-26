//bug free

#pragma once

#include <string>
#include <stringutil.h>
#include<enums.h>

class cookieParser
{
public:
    cookieParser(std::string);
    std::vector<COOKIE> cookies;
};

