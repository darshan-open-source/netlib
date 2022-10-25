#pragma once
#include <string>
#include <filesystem>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include "enums.h"
#ifndef UPLOADED_FILES_DIRECTORY
#define UPLOADED_FILES_DIRECTORY "/home/darshan/Documents/code/netlib(main)/files/"
#endif
#ifndef UPLOAD_FILE_SIZE
#define UPLOAD_FILE_SIZE 1048576
#endif
typedef std::string string;

class multipartLineParser
{
public:
  MULTIPART_DATA static parseLine(string s);
};
MULTIPART_DATA multipartLineParser::parseLine(string s)
{
  {

    int y = -2;
    int x = 0;
    MULTIPART_DATA data;
    data.isFile = false;
    while (1)
    {
      int x = s.find("\r\n", y + 2);
      if (x == std::string::npos)
      {
        return data;
      }
      std::vector<std::string> sp = split(s.substr(y + 2, x - (y + 2)), ';');
      for (size_t i = 0; i < sp.size(); i++)
      {
        std::vector<std::string> bs = split(sp.at(i), ':');
        if (bs.size() == 2)
        {
          while (1)
          {
            if (bs.at(0).at(0) == '\r' || bs.at(0).at(0) == '\n')
              bs.at(0).erase(bs.at(0).begin());
            else
              break;
          }

          if (toUpper(stripString(bs.at(0))).compare("CONTENT-DISPOSITION") == 0)
          {
            data.content_disposition = stripString(bs.at(1));
          }

          else if (toUpper(stripString(bs.at(0))).compare("CONTENT-TYPE") == 0)
          {

            data.content_type = stripString(bs.at(1));
          }
        }
        else
        {
          bs = split(sp.at(i), '=');
          if (toUpper(stripString(bs.at(0))).compare("NAME") == 0)
          {
            string s = stripString(bs.at(1));
            if (s.at(0) == '\"')
              s.erase(s.begin());
            if (s.at(s.length() - 1) == '\"')
              s.pop_back();
            data.name = s;
          }
          else if (toUpper(stripString(bs.at(0))).compare("FILENAME") == 0)
          {
            string s = stripString(bs.at(1));
            if (s.at(0) == '\"')
              s.erase(s.begin());
            if (s.at(s.length() - 1) == '\"')
              s.pop_back();
            data.isfilename = s;
            data.isFile = true;
          }
        }
      }

      y = x;
    }
  }
}
class multipartWriter
{
public:
  multipartWriter(MULTIPART_DATA *, string rid);
  ~multipartWriter()
  {


    
    if (d->isFile == true)
    {
      if (std::filesystem::file_size(std::filesystem::path(d->value)) <= 0)
        std::filesystem::remove(std::filesystem::path(d->value));
     
      close(fd);
    }
  }
  void writeData(string data);

private:
  MULTIPART_DATA *d;
  FILE *f;
  int fd;
};
multipartWriter::multipartWriter(MULTIPART_DATA *data, string rid)
{
  d = data;
  if (data->isFile == true)
  {
    string s = UPLOADED_FILES_DIRECTORY;
    s.append(rid);
    s.append("_");
    s.append(d->name);
    s.append("_");
    s.append(d->isfilename);
    // f = fopen(s.c_str(),"w");
    fd = open(s.c_str(), O_RDWR | O_CREAT, S_IWOTH | S_IROTH | S_IREAD);

    if (fd == 0)
    {
    }
    else
      d->value = s;
  }
}
void multipartWriter::writeData(std::string data)
{
  try{

  if (d->isFile == true)
  {
    if (fd != 0)
    {
      write(fd, data.c_str(), data.length()); // fwrite(data.c_str(),1,data.length(),f);
    }
  }
  else
  {
    d->value += data;
  }
  }
  catch(std::exception &e){
    cout << "error has been occured";
  }
}
