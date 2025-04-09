#pragma once

#include "Loader.h"

#include <unordered_map>

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_TEXTURES 1
#define ASSETS_SECTION_SPRITES 2
#define ASSETS_SECTION_ANIMATIONS 3

#define MAX_SCENE_LINE 1024

class SpritesLoader : public Loader {
private:
    unordered_map<string, string> textures;
    void _ParseSection_SPRITES(const string& line);
    void _ParseSection_TEXTURES(const string& line);
    void _ParseSection_ANIMATIONS(const string& line);
public:
    void Load(LPCWSTR path) override;
};