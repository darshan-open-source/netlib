#include<standalone.h>
void removeAllFilesFromMultipart(std::vector<MULTIPART_DATA> data)
{
    
    for (short i = 0; i < data.size(); i++)
    {
        

        auto x = data.at(i);
        if (x.isFile)
        {
            std::filesystem::remove(x.value);
        }
    }
}

bool isGetQuery(std::string path)
{
    if(path.find('?') != std::string::npos ){
    
        return true;
    }
    return false;
}
std::string getPathOfGetRequest(std::string path)
{
    return path.substr(0,path.find('?')!=std::string::npos?path.find('?'):path.length());

};

std::string _GET(std::multimap<std::string, std::string> payload, std::string name)
{

    std::multimap<std::string, std::string>::iterator it;
    for (it = payload.begin(); it != payload.end(); it++)
    {
        if (it->first.compare(name) == 0)
        {
            return it->second;
        }
    }
    string s;
    return s;
}
std::string getRandomSessionId(int length)
{
    string s;
    while (s.length() != length)
    {

        auto v = effolkronium::random_static::get('a', 'z');
        if (v != 46 && v != 47 && v != 92 && v != 59)
        {
            s.append(1, v);
        }
    }
    return s;
}

MIME_TYPE getFileTypeFromExtension(std::string filename)
{

    std::string s;
    if (std::filesystem::path(filename).has_extension())
    {
        s = std::filesystem::path(filename).extension().string();
        s.erase(s.begin());
    }
    else
    {
        return MIME_TYPE_UNDEFINED;
    }

    if (s.compare("txt") == 0)
    {
        return MIME_TYPE_text_plain;
    }
    else if (s.compare("mp4") == 0)
    {
        return MIME_TYPE_video_mp4;
    }
    else if (s.compare("mkv") == 0)
    {
        return MIME_TYPE_video_mkv;
    }
    else if (s.compare("bin") == 0)
    {
        return MIME_TYPE_octet_stream;
    }
    else if (s.compare("html") == 0)
    {
        return MIME_TYPE_text_html;
    }
    else if (s.compare("css") == 0)
    {
        return MIME_TYPE_text_css;
    }
    else if (s.compare("htm") == 0)
    {
        return MIME_TYPE_text_html;
    }
    else if (s.compare("js") == 0)
    {
        return MIME_TYPE_text_javascript;
    }
    else if (s.compare("bmp") == 0)
    {
        return MIME_TYPE_image_bmp;
    }
    else if (s.compare("jpeg") == 0)
    {
        return MIME_TYPE_image_jpeg;
    }
    else if (s.compare("png") == 0)
    {
        return MIME_TYPE_image_png;
    }
    else if (s.compare("pdf") == 0)
    {
        return MIME_TYPE_application_pdf;
    }
    else if (s.compare("xml") == 0)
    {
        return MIME_TYPE_application_xml;
    }
    else if (s.compare("json") == 0)
    {
        return MIME_TYPE_application_json;
    }
    else if (s.compare("gz") == 0)
    {
        return MIME_TYPE_application_gzip;
    }
    else if (s.compare("svg") == 0)
    {
        return MIME_TYPE_image_svg_xml;
    }
    else if (s.compare("webp") == 0)
    {
        return MIME_TYPE_image_webp;
    }
    else if (s.compare("xhtml") == 0)
    {
        return MIME_TYPE_application_xhtml_xml;
    }
    else if (s.compare("mjs") == 0)
    {
        return MIME_TYPE_text_javascript;
    }
    else if (s.compare("webm") == 0)
    {
        return MIME_TYPE_video_webm;
    }
    else if (s.compare("mkv") == 0)
    {
        return MIME_TYPE_video_mkv;
    }

    return MIME_TYPE_UNDEFINED;
}
std::string decodeUrl(std::string url)
{
    for (int i = 0; i < url.length(); i++)
    {
        if (url.at(i) == '+')
        {
            url.at(i) = ' ';
        }
    }
    CURL *curl = curl_easy_init();
    if (curl)
    {
        std::string s;
        int len;
        char *output = curl_easy_unescape(curl, url.c_str(), url.length(), &len);
        if (len)
        {
            s.append(output);
            curl_free(output);
        }
        curl_easy_cleanup(curl);

        return s;
    }
    
    return std::string();
}