#pragma once

#include <string>
#include <wtypes.h>
using namespace std;

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_ALIAS 1
#define ASSETS_SECTION_SPRITES 2
#define ASSETS_SECTION_ANIMATIONS 3

#define MAX_SCENE_LINE 1024

class Loader {
public:
    virtual void Load(LPCWSTR path) = 0;
};