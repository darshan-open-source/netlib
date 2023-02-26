// bug free
// currently _GET is not used in anywhere 
#pragma once
#include <string>
#include <string.h>
#include <map>
#include <effolkronium/random.hpp>
#include <filesystem>
#include <curl/curl.h>
#include <enums.h>

void removeAllFilesFromMultipart(std::vector<MULTIPART_DATA> data);
bool isGetQuery(std::string path);
std::string getPathOfGetRequest(std::string path);

std::string _GET(std::multimap<std::string, std::string> payload, std::string name);
std::string getRandomSessionId(int length);
MIME_TYPE getFileTypeFromExtension(std::string filename);
std::string decodeUrl(std::string url);