//bug free

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

private:
    bool error = false;
    FILE *f;
    nlohmann::json j;
};

#endif