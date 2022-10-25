//bug free

#pragma once

#include <ctime>
#include <string>

class currentDateTime {
public:
    std::string getTime() {
        notice();
        strftime(timestr, 9, "%T", timein);
        return std::string(timestr);
    };

    std::string getResponseTime();

private:
    void notice();

    char timestr[9];
    time_t rawtime;
    struct tm *timein;
};


void currentDateTime::notice() {
    time(&rawtime);
    timein = localtime(&rawtime);
};

std::string currentDateTime::getResponseTime() {
    notice();
    strftime(timestr, 30, "%a, %d %b %G %T %Z", timein);
    return std::string(timestr);
}