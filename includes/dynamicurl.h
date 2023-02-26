#include <iostream>
#include "url.h"
#include <regex>
using namespace std;
typedef std::filesystem::path::iterator pathIterator;


class dynamicPath
{
public:
    dynamicPath();
    bool isValidPath(std::string,std::string);
    std::vector<URL_DATA> getPathData() { return datax; };
     int isvalidatatype(string);

private:
    std::filesystem::path format;
    std::vector<urlDataType> predefdatatypes = {stringDataType(), intDataType(),alphanumDataType(), floatDataType(),  pathDataType(),slugDataType()};
    std::vector<URL_DATA> datax;
};
