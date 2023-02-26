#include<url.h>



//above this is function is not tested for bugs
int  isUrlDataType(std::string s)
{
    string first, second;
    std::vector<urlDataType> predefdatatypes = {stringDataType(), intDataType(),alphanumDataType(), floatDataType(),  pathDataType(),slugDataType()};

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

vector<string> splitPath(string path)
{

    filesystem::path p(path);
    vector<string> vc;
    for (auto i = p.begin(); i != p.end(); i++)
    {
        filesystem::path px(*i);
        if (px.string().compare("") == 0)
            vc.push_back("/");
        else
            vc.push_back(*i);
    }
    return vc;
};
vector<string> splitPathNormal(string path)
{

    filesystem::path p(path);
    vector<string> vc;
    for (auto i = p.begin(); i != p.end(); i++)
    {
        filesystem::path px(*i);
       
            vc.push_back(*i);
    }
    return vc;
};


short isDynamicUrl(std::string url)
{

    vector<string> vc = splitPath(url);
    for (int i = 0; i < vc.size(); i++)
    {
        if (isUrlDataType(vc.at(i)))
        {
            return 1;
        }
    }

    return 0;
}

URL_DYNAMIC_DATA getDynamicUrlData(std::string path)
{
    URL_DYNAMIC_DATA data;

    int total = 0;
    int start = -1;
    int blocks=0;
    vector<string> vc = splitPath(path);
    for (int i = 0; i < vc.size(); i++)
    {
        if (isUrlDataType(vc.at(i)))
        {
            total++;
            if (start == -1)
                start = i;
        }
        blocks++;
    }
    data.start = start;
    data.total = total;
    data.blocks = blocks;
    return data;
}

std::string getDynamicBasicString(std::string dynamic_string)
{


    string s;
    vector<string> vc = splitPath(dynamic_string);



    int x = getDynamicUrlData(dynamic_string).start != -1 ? getDynamicUrlData(dynamic_string).start : getDynamicUrlData(dynamic_string).blocks;
    for (int i = 0; i < x; i++)
    {

        // cout << " block "<< vc.at(i)<<endl;
        if(vc.at(i).compare("/") != 0){
        s.append(vc.at(i));

        }
        s.append("/");

    }
   // cout << s<<endl;
    return s;
};

std::string getVariableName(std::string s){
    string second;
    int sx = s.find(":");
    second = s.substr(sx + 1, s.length() - sx);
    second.erase(second.length()-1);
    

    return stripString(second);

}
std::string disableJs(std::string s){
    string sp;
    for(int i=0;i<s.length();i++){
        if(s.at(i)=='>'){
          sp += "&gt";
        }
        else if(s.at(i)=='<'){
          sp+="&lt";
        }
        else if(s.at(i)=='&'){
         sp+="&amp";
        }
        else{
            sp.append(1,s.at(i));
        }
    }
return sp;
}
