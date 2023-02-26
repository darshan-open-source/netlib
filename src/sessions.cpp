#include<sessions.h>
sessions::sessions(std::string filename)
{

    std::filesystem::path p(filename);

    f = fopen(p.c_str(), "a+");
    if (f == 0)
    {
            error = true;
            return;
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

    

    f = freopen(filename.c_str(), "w+", f);
    fwrite(j.dump().c_str(), 1, j.dump().length(), f);
    fflush(f);
}