
//memory leak error 
//need to free timein object

//solved but not tested
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
     ~currentDateTime(){
        // free(timein);
     }
private:
    void notice();

    char timestr[9];
    time_t rawtime;
    struct tm *timein;
};


