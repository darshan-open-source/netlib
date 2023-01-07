#ifndef SESSIONS_INCLUDED
#define SESSIONS_INCLUDED
#include <string>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <stdlib.h>

class sessions
{
public:
    sessions()
    {
        this->error = true;
    };
    ~sessions()
    {
        fclose(f);
    }

    sessions(std::string);

    nlohmann::json get(std::string);
    template <typename t>
    void set(std::string, t);

    std::string filename;
    bool error = false;

private:
    FILE *f;
    nlohmann::json j;
};
sessions::sessions(std::string filename)
{

    std::filesystem::path p(filename);

    f = fopen(p.c_str(), "a+");
    if (f == 0)
    {
        console::log("not able to open");
    }
    this->filename.assign(filename);

    uintmax_t filesize = std::filesystem::file_size(p);
    char *filebuf = (char *)malloc(filesize + 1);
    int x = fread(filebuf, 1, filesize, f);
    filebuf[filesize] = '\0';
    try
    {
        j = nlohmann::json::parse(filebuf);
    }
    catch (std::exception &e)
    {
    }

    free(filebuf);
}

nlohmann::json sessions::get(std::string key)

{

    if (j.contains(key))
    {
        nlohmann::json a = j[key];
        return a;
    }
    else
    {
        return nlohmann::json();
    }
}
template <typename t>
void sessions::set(std::string key, t value)
{
    if (error)
    {
        return;
    }
    j[key] = value;

    if (f == 0)
    {
        console::log("file is already close");
    }

    f = freopen(filename.c_str(), "w+", f);
    fwrite(j.dump().c_str(), 1, j.dump().length(), f);
    fflush(f);
}

#endif
