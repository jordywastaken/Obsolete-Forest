
#pragma once

#include "../Game/Structs.hpp"

extern float colorWhite[];
extern float colorBlack[];

extern int fontCount;
extern const char* fontPath[];
extern const char* fontName[];

class Globals
{
public:
    float colorAccentFadeTick{ 0 };

    /* Config */
    struct
    {
        struct
        {
            bool godMode{ true };
            float damageAttenuation{ 0 };
            bool unlimitedAmmo{ true };
            bool noClip{ false };

            bool viewModelWireframe{ false };
            vec3_t viewModelPos{ 0, 0, 0 };
        } main;

        struct
        {
            float posX{ 640 };
            float posY{ 100 };

            float sizeWidth{ 380 };
            float sizeOutline{ 2 };
            float sizeHeader{ 35 };
            float sizeSeparator{ 2 };
            float sizeTab{ 28 };
            float sizeOption{ 22 };
            float sizeOptionSpacing{ 0 };
            float sizeOptionStartX{ 6 };
            float sizeOptionStartY{ 2 };
            float sizeBox{ 16 };
            float sizeSliderHeight{ 6.5 };
            float sizeSliderWidth{ 100 };

            int font{ 0 };

            vec4_t colorBackground{ 0.105, 0.105, 0.105, 0.9 };
            vec4_t colorTab{ 0.1575, 0.1575, 0.1575, 0.5 };
            vec4_t colorSeparator{ 0.27, 0.27, 0.27, 1 };

            vec4_t colorAccentPrimary{ 0.08, 0.86, 0.45, 1 };
            vec4_t colorAccentSecondary{ 0.08, 0.45, 0.86, 1 };
            bool colorAccentFade{ false };
            float colorAccentFadeSpeed{ 0.1 };
            float colorAccentFadeHueSpacing{ 60 };

            bool infoboxDrawInstructions{ true };
            bool infoboxDrawWatermark{ true };
            bool infoboxDrawResolution{ true };
            bool infoboxDrawFps{ true };
            float infoboxPosX{ 20 };
            float infoboxPosY{ 20 };
            float infoboxSizeSpacing{ 2 };
            float infoboxSizeHeight{ 20 };
            float infoboxSizeOutline{ 2 };
        } settings;
    } config;
};

extern Globals* g_Globals;