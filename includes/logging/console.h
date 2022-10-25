#pragma once
#include<iostream>
class console
{
public:
    static void log(std::string s);
    static void log(int);
};
void console::log(std::string s)
{
    std::cout << std::endl
              << s;
    fflush(stdout);
}
void console::log(int x){
    std::cout << std::endl << x;
    fflush(stdout);
}