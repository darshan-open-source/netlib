#pragma once
#include<iostream>
#include <string>
#include <string.h>
#include <sstream>
#include <vector>
#include <regex>
#include <algorithm>
#include <map>
#define STRING_MATHCHED 0
#define STRING_UNMATCHED 1
typedef std::string string;

using namespace std;
int compareString(std::string str1, std::string str2);
void printStringln(std::string s);
void printStringln(char *x, int len);
void printString(string s);
void printString(char *x, int len);
string reverseString(string st);
std::string stripString(std::string str);
string toUpper(string st);
vector<std::string> split(std::string s, char delimeter);
void displayMap(std::map<string, string> map);
void displayMap(std::multimap<string, string> map);
typedef struct __FIND_BOUNDARY_RESULT
{
    bool full;
    bool found;
    int length;
    int start;
} FIND_BOUNDARY_RESULT;
FIND_BOUNDARY_RESULT findBoundary(string st, string boundary);

void printwithescape(char *f);
bool isString(std::string st);
bool isNumber(std::string st);
bool matchRegex(string regexx, string value);