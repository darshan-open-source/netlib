
#pragma once
#include <fstream>

class fileIO
{
public:
   fileIO(std::string);
   void write(std::string);
   void operator<<(std::string);
private:
   std::fstream f;

};

fileIO::fileIO(std::string filename)
{
   f.open(filename, std::ios_base::out);
}

void fileIO::write(std::string data)
{
   f << data;
}
void fileIO::operator<<(std::string text)
{
   f << text;
   f.flush();
}

class defaultLog
{
   defaultLog();
   size_t write();
   size_t writern();
   size_t writeln();
   private:
   std::fstream f;
};

defaultLog::defaultLog(){
      
}
