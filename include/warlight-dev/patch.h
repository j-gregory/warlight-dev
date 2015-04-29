#include <string>
#include <stdio.h>
#include <iostream>
#include <sstream>

using namespace std;

namespace patch
{
    template <typename T> std::string to_string(const T& n)
    {
        std::ostringstream stm;
        stm << n;
        return stm.str();
    }
}










