#include<httpResponse.h>

 
RESPONSE httpResponse::HttpResponse(std::string value)
{

    std::string valx = "<html><body>";
    valx += value;

    valx += "</body></html>";
    RESPONSE r;

    r.type = 0;
    r.val = valx;
    r.json = 0;

    r.header = getHeader();
    return r;
}
RESPONSE httpResponse::RawResponse(std::string value)
{
    
    RESPONSE r;

    r.type = 0;
    r.val = value;
    r.json = 0;


    r.header = getHeader();
    return r;
}
RESPONSE httpResponse::HttpRedirect(REDIRECT_TYPE r, std::string url)
{
    setHttpStatusCode((HTTP_STATUS_CODE)r);

    set("Location", url);
    RESPONSE rmp;

    rmp.type = 0;
    rmp.json = 0;


    rmp.header = getHeader();
    return rmp;
};

void httpResponse::sendHeader()
{
    writer->writeString(getHeader());
};
RESPONSE httpResponse::render(std::string filename, inja::json j, bool usestaticdir)
{

    std::string v;

    if (usestaticdir == true)
    {
        v.append(defaultStaticDirectory);;
    }

    v.append(filename);
    RESPONSE res;
    res.type = 1;

    res.header = getHeader();
    res.val = v;
    res.json = j;

    return res;
};
sessions httpResponse::createSession()
{
    COOKIE c;
    c.sameSite=SAMESITE_TYPE_NONE;
    c.name = SESSION_ID;
    c.httpOnly = true;
    c.secure = true;
    while (1)
    {

        string sid = getRandomSessionId(SESSION_VALUE_SIZE);
        string s = SESSION_ID;
        s += "_";
        s += sid;
        s += ".json";
        std::filesystem::path pth(session_directory);

        pth.replace_filename(std::filesystem::path(s));

        if (!std::filesystem::exists(pth))
        {

            FILE *f = fopen(pth.c_str(), "w+");

            if (f != 0)
            {

                c.value = sid;
                setCookie(c);
                fclose(f);
                return sessions(pth.string());
            }
            else
            {
                perror("error");
            }
        }
    }
};

void httpResponse::expireSession()
{
    cookieParser cp(h.cookieString);

    std::string name = SESSION_ID;

    std::vector<COOKIE> c = cp.cookies;
    std::string X;
    for (int i = 0; i < c.size(); i++)
    {
        if (name.compare(c.at(i).name) == 0)
        {
            X = c.at(i).value;
        }
    }

    if (!X.empty())
    {

        string s = SESSION_ID;
        s += "_";
        s += X;
        s += ".json";
        std::filesystem::path p(session_directory);
        p.replace_filename(std::filesystem::path(s));
         remove(p);
        COOKIE c;
        c.name = SESSION_ID;
        c.httpOnly = true;
        c.secure = true;
        setCookie(c);
    }
}