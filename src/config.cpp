#include<config.h>
std::string getFileContent(std::string filename)
{

  size_t size = std::filesystem::file_size(filename);

  FILE *f = fopen(filename.c_str(), "r");

  std::string s;
  s.resize(size);
  fread(s.data(), 1, size, f);

  fclose(f);
  return s;
}
std::string readValueFromDefaultConfig(std::string name)
{
  std::vector<std::string> lines = split(getFileContent("settings.conf"), '\n');

  for (std::string line : lines)
  {
    std::vector<std::string> linex = split(line, '=');
    if (linex.at(0).compare(name) == 0)
    {
      return linex.at(1);
    }
  }

  return std::string();
};
