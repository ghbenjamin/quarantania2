#pragma once

#include <string>
#include <vector>

struct ManifestImageData
{
    std::string key;
    std::string path;
};

struct ManifestSpritesheetData
{
    std::string key;
    std::string path;

    int margin;
    int tileSize;
};

struct ManifestFontData
{
    std::string key;
    std::string path;

    int fontSize;
};

struct ManifestData
{
    std::vector<ManifestImageData> images;
    std::vector<ManifestSpritesheetData> spriteSheets;
    std::vector<ManifestFontData> fonts;
};