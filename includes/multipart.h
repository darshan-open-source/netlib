#pragma once
#include <string>
#include <filesystem>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <standalone.h>
#include<stringutil.h>
#include "enums.h"

#ifndef UPLOAD_FILE_SIZE
#define UPLOAD_FILE_SIZE 1048576
#endif
#include<config.h>
typedef std::string string;

static std::string uploadedFilesDirectory = readValueFromDefaultConfig("uploaded_files_directory");

class multipartLineParser
{
public:
  MULTIPART_DATA static parseLine(string s);
};

class multipartWriter
{
public:
  multipartWriter(MULTIPART_DATA *, string rid);
  ~multipartWriter()
  {

    if (d->isFile == true)
    {

      if (f != 0)
      {
        fflush(f);
        // try{
        // if (std::filesystem::file_size(std::filesystem::path(d->value)) <= 0)
        //  {
        //  std::filesystem::remove(std::filesystem::path(d->value));
        //  }
        // }

        fclose(f);
      }
    }
  }
  void writeData(string data);

private:
  MULTIPART_DATA *d;
  FILE *f;
  int fd;
};
