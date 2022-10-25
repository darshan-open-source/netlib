#include <iostream>
#include "url.h"
#include <regex>
using namespace std;
typedef std::filesystem::path::iterator pathIterator;

int findBlockNumber(pathIterator start, pathIterator end, std::string value)
{

    int count = 0;
    for (pathIterator i = start; i != end; i++)
    {

        if (strcmp(value.c_str(), (*i).c_str()) == 0)
        {

            return count;
        }
        count++;
    }
    return -1;
}
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
int dynamicPath::isvalidatatype(std::string s)
{
    string first, second;
    int sx = s.find(":");
    if (sx == std::string::npos)
        return false;
    first = s.substr(0, sx);
    second = s.substr(sx + 1, s.length() - sx);
    if (second.at(second.length() - 1) == '>')

    {
        if (first.at(0) == '<')
        {
            string sss = stripString(first.substr(1, first.length()));
            for (int i = 0; i < predefdatatypes.size(); i++)
            {
                if (sss.compare(predefdatatypes.at(i).xtypename) == 0)
                    return predefdatatypes.at(i).id;
            }
        }
    }
    return 0;
}
dynamicPath::dynamicPath(){};

bool dynamicPath::isValidPath(std::string path,std::string format)
{
    int find = 1;
    std::filesystem::path pathx(path);
    std::vector<std::string> formatPath = splitPathNormal(format.c_str());
    pathIterator pathItertorx = pathx.begin();
    for (int i = 0; i < formatPath.size(); i++)
    {
        std::string ivalue = formatPath[i];
        if (pathItertorx == pathx.end())
        {

            find = 0;
            break;
        }
        int id =this->isvalidatatype(formatPath[i]);
        if(id==0)
        {

            if (strcmp(ivalue.c_str(), (*pathItertorx).c_str()) == 0)
            {

                pathItertorx++;
            }
            else
            {

                find = 0;
                break;
            }
        }
        else
        {
            URL_DATA temp_data;
            temp_data.dataType = id;

           
            temp_data.name = getVariableName(formatPath[i]);





            if (id == 5)
            {

                
                int findlocation = 0;
                if (i != formatPath.size() - 1)
                {
                    

                    findlocation = findBlockNumber(pathItertorx, pathx.end(), formatPath[i + 1]);
                    if (findlocation == -1)
                    {
                      
                        find = 0;
                        break;
                    }
            
                }
                else
                {
                
                    findlocation = distance(pathItertorx, pathx.end());
                }
                std::filesystem::path px;

                for (int il = 0; il < findlocation; il++)
                {

                    px /= *pathItertorx;
                    temp_data.value = px.c_str();
                    pathItertorx++;
                }
            }
            else
            {
                if (matchRegex(predefdatatypes[id-1].xregex, (*pathItertorx).c_str()) != 1)
                {
                    find = 0;
                    break;
                }
                else
                {

                    temp_data.value = *pathItertorx;
                    pathItertorx++;
                }
            }
            if (find == 1)
                datax.push_back(temp_data);
        }
    }
    if (distance(pathItertorx, pathx.end()) != 0)
        find = 0;


    return find;
}

