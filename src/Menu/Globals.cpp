
#include "Globals.hpp"

float colorWhite[]{ 1, 1, 1, 1 };
float colorBlack[]{ 0, 0, 0, 1 };

int fontCount = 7;
const char* fontPath[]
{
    "fonts/720/normalFont",
    "fonts/720/smallFont",
    "fonts/720/extraSmallFont",
    "fonts/720/bigFont",
    "fonts/720/extraBigFont",
    "fonts/720/consoleFont",
    "fonts/smallDevFont",
    "fonts/bigDevFont"
};
const char* fontName[]
{
    "Normal",
    "Small",
    "Extra small",
    "Big",
    "Extra big",
    "Console",
    "Dev small",
    "Dev big"
};

Globals* g_Globals;