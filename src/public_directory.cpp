#include<public_directory.h>

publicDirectory::publicDirectory(std::string path, httpResponse res,std::string baseurl)
{

    for (auto const &dir_entry : std::filesystem::directory_iterator{std::filesystem::path(path)})
    {
        if (dir_entry.is_regular_file())
        {
            fname.push_back(dir_entry.path().filename().string());

            std::time_t cftime = std::chrono::system_clock::to_time_t(
                std::chrono::file_clock::to_sys(dir_entry.last_write_time()));

            char timestr[30];
            char datestr[11];
            std::strftime(timestr, 30, "%r %Z", std::localtime(&cftime));
            std::strftime(datestr, 11, "%d/%m/%Y", std::localtime(&cftime));

            ftime.push_back(timestr);
            fdate.push_back(datestr);

            fsize.push_back(getfilesize(dir_entry.file_size()));
        }
    }

    j["burl"] = baseurl;
    j["fname"] = fname;
    j["fsize"] = fsize;
    j["ftime"] = ftime;
    j["fdate"] = fdate;
}
RESPONSE publicDirectory::getResponse() {
     RESPONSE  r;
     r.json = j;
     r.type = 1;
     r.header = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";
     r.val = "/home/darshan/Documents/code/netlib(main)/static/public_directory.html";

    return r;
}

class publicDirectoryRecursive
{
public:
    publicDirectoryRecursive(std::string, httpResponse);

private:
    std::vector<string> fname;
    std::vector<string> ftime;
    std::vector<string> fdate;

    std::vector<string> fsize;
    std::vector<bool> isdir;
    string getfilesize(uintmax_t y)
    {
        string s;
        int i{};

        double mantissa = y;
        for (; mantissa >= 1024.; ++i)
        {
            mantissa /= 1024.;
        }
        mantissa = std::ceil(mantissa * 10.) / 10.;
        s.append(to_string((int)mantissa));

        s += "BKMGTPE"[i];
        return s;
    }
};
publicDirectoryRecursive::publicDirectoryRecursive(std::string path, httpResponse res)
{

    for (auto const &dir_entry : std::filesystem::directory_iterator{std::filesystem::path(path)})
    {
        fname.push_back(dir_entry.path().filename().string());

        std::time_t cftime = std::chrono::system_clock::to_time_t(
            std::chrono::file_clock::to_sys(dir_entry.last_write_time()));

        char timestr[30];
        char datestr[11];
        std::strftime(timestr, 30, "%r %Z", std::localtime(&cftime));
        std::strftime(datestr, 11, "%d/%m/%Y", std::localtime(&cftime));

        ftime.push_back(timestr);
        fdate.push_back(datestr);

    

        if (dir_entry.is_directory())
        {
            isdir.push_back(true);
            fsize.push_back(" ");
        }
        else
        {

        fsize.push_back(getfilesize(dir_entry.file_size()));
            isdir.push_back(false);
        }
    }
    nlohmann::json j;
    j["fname"] = "w";
    // j["fsize"] = fsize;
    // j["ftime"] = ftime;
    // j["fdate"] = fdate;
    // j["isdir"] = isdir;
   // j["base"] = base;
    // cout << "ch1"<<endl;
   // j["isbase"]=base.length();
    // cout << "ch2"<<base.length()<<endl;
    res.render("/home/darshan/Documents/code/private web/public_directory_recursive.html",j);
        // cout << "chx"<<base.length()<<endl;

}