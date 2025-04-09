#pragma once

#include <string>
#include <wtypes.h>
using namespace std;


class Loader {
public:
    virtual void Load(LPCWSTR path) = 0;
};