#include<server_time.h>

void currentDateTime::notice() {
    time(&rawtime);
    timein = localtime(&rawtime);
};

std::string currentDateTime::getResponseTime() {
    notice();
    strftime(timestr, 30, "%a, %d %b %G %T %Z", timein);
    return std::string(timestr);
}