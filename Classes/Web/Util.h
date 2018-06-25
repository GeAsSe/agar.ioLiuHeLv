#pragma once

#include <list>
#include <string>
using namespace std;

class Util
{
public:
    static void Explode(const char *lpszSrc, const char *lpszDelimiter, std::list<std::string> &subList);
};
