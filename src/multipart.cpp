#include<multipart.h>
MULTIPART_DATA multipartLineParser::parseLine(string s)
{

  int y = -2;
  int x = 0;
  MULTIPART_DATA data;
  data.value ="";
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
          data.filename = s;
          data.isFile = true;
          // console::log("filename has been found");
        }
      }
    }

    y = x;
  }
}
multipartWriter::multipartWriter(MULTIPART_DATA *data, string rid)
{
  d = data;
  if (data->isFile == true)
  {
    string s;
    while (1)
    {
      /* code */
      s = uploadedFilesDirectory;

      s.append(getRandomSessionId(10));
      s.append("_");
      s.append(d->name);
      s.append("_");
      s.append(d->filename);
      if (!std::filesystem::exists(s))break;
      
    }
    f = fopen(s.c_str(), "wb");
    if (f == NULL)
    {
      perror("error");
      // logv << "error\n";
    }
    else
      d->value = s;
  }
}
void multipartWriter::writeData(std::string data)
{
  if (d->isFile == true)
  {
    if (f != 0)
    {
      fwrite(data.c_str(), 1, data.length(), f);
    }
  }
  else
  {


    d->value += data;
  }
}
