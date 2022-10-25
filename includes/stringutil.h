#pragma once
#include <string>
#include <string.h>
#include <sstream>
#include<vector>
#include"logging/console.h"
#include<regex>
#include<algorithm>
#include<map>
#define STRING_MATHCHED 0
#define STRING_UNMATCHED 1
typedef std::string string;

using namespace std;
int compareString(std::string str1, std::string str2)
{
    if (strcmp(str1.c_str(), str2.c_str()) == 0)
        return STRING_MATHCHED;
    else
        return STRING_UNMATCHED;
}
void printStringln(std::string s)
{
    printf("\n%s", s.c_str());
}
void printStringln(char *x, int len)
{
    string s(x, len);
    printStringln(s);
}
void printString(string s)
{
    printf("%s", s.c_str());
}
void printString(char *x, int len)
{
    string s(x, len);
    printString(s);
}
string reverseString(string st)
{
    string reverseString;
    for (string::reverse_iterator i = st.rbegin(); i != st.rend(); i++)
        reverseString += *i;
    return reverseString;
}
std::string stripString(std::string str)
{
    string mainstring = str;
    while (1)
    {
        if (mainstring.at(0) == ' ')
        {
            mainstring.erase(mainstring.begin());
        }
        else
            break;
    }
    string s = reverseString(mainstring);
    while (1)
    {
        if (s.at(0) == ' ')
        {

            s.erase(s.begin());
        }
        else
            break;
    }

    return reverseString(s);
};
string toUpper(string st)
{
    string s;
    for (int i = 0; i < st.length(); i++)
    {
        s += ((char)toupper(st.at(i)));
    }

    return s;
}
vector<std::string> split(std::string s, char delimeter)
{
    vector<std::string> tokens;

    stringstream sp(s.c_str());

    string intermediate;

    while (getline(sp, intermediate, delimeter))
    {
        tokens.push_back(intermediate);
    }
    return tokens;
}
void displayMap(std::map<string, string> map)
{
    std::map<string, string>::iterator it;
    for (it = map.begin(); it != map.end(); it++)
    {
        cout << endl
             << it->first << "   " << it->second;
    }
}
void displayMap(std::multimap<string, string> map)
{
    std::multimap<string, string>::iterator it;
    for (it = map.begin(); it != map.end(); it++)
    {
        cout 
             << it->first << "   " << it->second<<std::endl;
    }

}

typedef struct __FIND_BOUNDARY_RESULT{
bool full;
bool found;
int length;
}FIND_BOUNDARY_RESULT;
FIND_BOUNDARY_RESULT findBoundary(string st, string boundary)
{
    FIND_BOUNDARY_RESULT r;
    r.full =r.found=false;
    r.length =-1;
    int x = -1;
    while (1)
    {
        x = st.find(boundary.at(0), x+1);
        if (x != std::string::npos)
        {

            int loop = 0;
            int j;
            if(st.length()-x>=boundary.length()){
                j=boundary.length();

            }else j= st.length()-x;
            for (int i = x ; i < (j+x); i++)
            {
                if (st.at(i) == boundary.at(loop))
                {
                    loop++;
                }
                else
                {
                    break;
                }
            }
            if (loop == j )
            {

                if(loop==boundary.length()){
                    r.full=true;
                }
                
                r.found=true;
                r.length = boundary.length()-loop;

                return r;
            }
            else
            {
                continue;
            }
        }
        else{

            break;}
    }
    return r;
}

short find(const char *string, int length, char c)
{
    for (int i = 0; i < length; i++)
    {
        if (string[i] == c)
        {
            return i;
        }
    }

    return -1;
}
short find(const char *string, int length, char c, int afterpos = 0)
{
    for (int i = afterpos + 1; i < length; i++)
    {
        if (string[i] == c)
        {
            return i;
        }
    }
    return -1;
}
int findString(const char *st, int length, std::string tobefind, int afterpos = 0)
{
    
    console::log("checksum middle");
    if (length < tobefind.length())
    

        return -1;
    short x = afterpos;
    while (1)
    {
        x = find(st, length, tobefind.at(0), x);

        if (x == -1)
        {
            return -1;
        }
        if (length - x >= tobefind.length())
        {
            short count = 0;
            for (int i = x; i < x + tobefind.length(); i++)
            {
                if (st[i] == tobefind[count])
                {
                    count++;
                }
                else
                    break;
            }
            if (count == tobefind.length())
            {
                return x;
            }
        }
    }

    return -1;
}
int findString(std::vector<char> v, std::string tobefind, size_t afterpos = 0)
{
console::log("ch 10");
//  const char *crlf2 = "\r\n\r\n";

auto it = std::search(v.begin(), v.end(),tobefind.data(),tobefind.data()+tobefind.length());


size_t x =std::distance(v.begin(), it);    

if(x==v.size())return -1;
else return x;  
}



void printwithescape(char *f){
    for(int i=0;i<strlen(f);i++){
      if(f[i]=='\r')cout << "\\r";
      else if(f[i] == '\n')cout << "\\n"<<endl;
      else cout << f[i];  
      
    }
}


bool isString(std::string st){
   for(int i=0;i<st.size();i++){
    if(!(std::isalpha(st[i]) || std::ispunct(st[i])))return false;
   }

   return true;
}


bool isNumber(std::string st){
   for(int i=0;i<st.size();i++){
    if(!std::isdigit(st[i]))return false;
   }

   return true;
}
bool matchRegex(string regexx,string value){

 
    regex b(regexx);
 
    if ( regex_match(value, b) )return true;



    return false;

}