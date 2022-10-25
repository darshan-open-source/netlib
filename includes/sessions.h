//bug free get/set function set
#ifndef SESSIONS_INCLUDED
#define SESSIONS_INCLUDED
#ifndef SESSION_DIRECTORY
#define SESSION_DIRECTORY "/home/darshan/Documents/code/netlib(main)/sessions/"
#endif
#include <string>
#include <sstream>
#include <filesystem>
#include <rapidjson/rapidjson.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>

#ifndef BLOCK_SIZE
#define BLOCK_SIZE 100000
#endif

void createJson(FILE *f,std::string key,std::string value){
    
    if(f == 0){

      return;
    }
        

     rapidjson::Value v1(key.c_str(),key.length());
     rapidjson::Value v2(value.c_str(),value.length());
     rapidjson::Document dc;
     dc.SetObject();
     dc.AddMember(v1,v2,dc.GetAllocator());
     char buffer[1000];
     rapidjson::StringBuffer sb;
     rapidjson::Writer<rapidjson::StringBuffer> writerc(sb);
     dc.Accept(writerc);
    

    fwrite(sb.GetString(),1,sb.GetLength(),f);
    fflush(f);
}
class sessions
{
public:
    sessions(int,std::string);
    sessions(){};
    sessions(std::string);
    std::string get(std::string);
    void set(std::string, std::string);
    std::string filename;

private:
    int fd;
    FILE *f;
};
sessions::sessions(int fd,std::string filename){
    this->fd = fd;
    this->filename = filename;
}
sessions::sessions(std::string filename)
{
    
    std::filesystem::path p(filename);
    




    f = fopen(p.c_str(),"a+");
    this->filename.assign(filename);

}

std::string sessions::get(std::string key)
{
    if(f==0) return std::string();
    char buf[BLOCK_SIZE];
    fseek(f,0,SEEK_SET);

    size_t x = fread(buf,1,BLOCK_SIZE,f);
    std::string s(buf, x);

    rapidjson::Document d;
    d.Parse(s.c_str());
    if (d.HasMember(key.c_str()))
    {

        rapidjson::Value &v = d[key.c_str()];
        return v.GetString();
    }
    else
    {
        std::string ss;
        return ss;
    }
}
void sessions::set(std::string key, std::string value)
{
    if(f==0) return;
    char buffer[BLOCK_SIZE];
    rapidjson::Document d;
    char bf[10000];
        

    fseek(f,0,SEEK_SET);


    fread(bf,1,10000,f);
    std::string s(bf);
    d.Parse(s.c_str());




   if (d.HasParseError())
    {


        createJson(f, key, value);
   }
  
    else
    {

        if (d.HasMember(key.c_str()))
        {
            rapidjson::Value &v = d[key.c_str()];
            v.SetString((char *)value.c_str(), value.length());
        }
        else
        {


            rapidjson::Value key2(key.c_str(), key.length());
            rapidjson::Value val2(value.c_str(), value.length());
            d.AddMember(key2, val2, d.GetAllocator());
        }
        rapidjson::StringBuffer sb;
        rapidjson::Writer<rapidjson::StringBuffer> w(sb);
        d.Accept(w);
        fclose(f);

        f = fopen(filename.c_str(),"w+");
        if(f==0){
            perror(filename.c_str());
        }

       fseek(f,0,SEEK_SET);

       size_t x = fwrite(sb.GetString(),1,sb.GetLength(),f);
       fflush(f);
     }
}

#endif