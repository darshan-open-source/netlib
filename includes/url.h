
//bug free
//  This module is used for dynamic url

#ifndef URLS_INCLUDED
#define URLS_INCLUDED
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <debug.h>
#include <stringutil.h>
using namespace std;

class urlDataType{
    public:
    urlDataType(std::string typenames,std::string regex,int id):xtypename(typenames),xregex(regex){this->id=id;};
    std::string xtypename;
    std::string xregex;
    unsigned short id;
};

 class stringDataType:public urlDataType{
    public:
    stringDataType():urlDataType("string","[a-zA-Z]*",1){};
 } ;

class intDataType:public urlDataType{
   public:
   intDataType():urlDataType("int","[0-9]*",2){}
};

class alphanumDataType:public urlDataType{
public:
   alphanumDataType():urlDataType("alphanum","[a-zA-Z0-9]*",3){};
};

class floatDataType:public urlDataType{
   public:
   floatDataType():urlDataType("float","[0-9]+[.]?[0-9]+[f]?",4){};
};

class pathDataType:public urlDataType{
    public:
    pathDataType():urlDataType("path","[a-zA-Z]*",5){};
};

class slugDataType:public urlDataType{
public:
    slugDataType():urlDataType("slug","[\x20-\x7E]*",6){};
};


typedef struct __URL_DYNAMIC_DATA
{
    int start;
    int total;
    int blocks;
} URL_DYNAMIC_DATA;

typedef struct _URL_DATA
{
    int dataType;
    std::string name;
    std::string value;

} URL_DATA;


//above this is function is not tested for bugs
int  isUrlDataType(std::string s);

vector<string> splitPath(string path);
vector<string> splitPathNormal(string path);


short isDynamicUrl(std::string url);

URL_DYNAMIC_DATA getDynamicUrlData(std::string path);
std::string getDynamicBasicString(std::string dynamic_string);

std::string getVariableName(std::string s);
std::string disableJs(std::string s);

#endif