#pragma once

#include "Loader.h"

#include <unordered_map>

class SpritesLoader : public Loader {
private:
    unordered_map<string, string> alias;
    void _ParseSection_SPRITES(const string& line);
    void _ParseSection_ALIAS(const string& line);
    void _ParseSection_ANIMATIONS(const string& line);
public:
    void Load(LPCWSTR path) override;
};