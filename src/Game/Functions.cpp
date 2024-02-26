
#include "Functions.hpp"
#include "../Utils/Memory.hpp"
#include <fastmath.h>
#include <algorithm>

/* Game Imports */
cell::Gcm::Inline::CellGcmContext& g_gcmContext = *(cell::Gcm::Inline::CellGcmContext*)(0x2220600);

unsigned int Game_Atan2_fptr[]{ 0x2F9EF0, GameToc };
auto Game_Atan2 = reinterpret_cast<double(*)(double y, double x)>(Game_Atan2_fptr);

unsigned int Game_Fmodf_fptr[]{ 0x68B600, GameToc };
auto Game_Fmodf = reinterpret_cast<float(*)(float x, float y)>(Game_Fmodf_fptr);

unsigned int Game_Printf_fptr[]{ 0x67E8B0, GameToc };
auto Game_Printf = reinterpret_cast<int(*)(const char* fmt, ...)>(Game_Printf_fptr);

unsigned int va_fptr[]{ 0x3B7298, GameToc };
auto va = reinterpret_cast<const char*(*)(const char* fmt, ...)>(va_fptr);

void I_strncpyz(char* dest, const char* src, int destSize)
{
    GameCall<void>(0x3B6394, dest, src, destSize);
}

int I_stricmp(const char* str1, const char* str2)
{
    return GameCall<int>(0x3B6B58, str1, str2);
}

int Sys_Milliseconds()
{
    return GameCall<int>(0x302CE8);
}

GfxCmdHeader* R_GetCommandBuffer(int renderCmd, int bytes)
{
    return GameCall<GfxCmdHeader*>(0x5B062C, renderCmd, bytes);
}

GfxCmdDrawText2D* R_AddCmdDrawTextInternal(const char* text, int maxChars, Font* font, float x, float y, float xScale, float yScale, float rotation, vec4_t* color, int style)
{
    return GameCall<GfxCmdDrawText2D*>(0x5B0EA0, text, maxChars, font, x, y, xScale, yScale, rotation, color, style);
}

void R_AddCmdSetScissorValuesInternal(bool enabled, int x, int y, int width, int height)
{
    GameCall<void>(0x5B1D1C, enabled, x, y, width, height);
}

int R_TextWidth(int localClient, const char* text, int maxChars, Font* font)
{
    return GameCall<int>(0x5A08D8, localClient, text, maxChars, font);
}

const char* R_GetFontPathFromName(const char* name)
{
    return GameCall<const char*>(0x5A261C, name);
}

void DB_FindXAssetHeader(XAsset* asset, XAssetType type, const char* name, bool errorIfMissing, int waitTime)
{
    GameCall<void>(0x1690D4, asset, type, name, errorIfMissing, waitTime);
}

ScreenPlacement* ScrPlace_GetView(int localClient)
{
    return GameCall<ScreenPlacement*>(0x13D820, localClient);
}

void CG_DrawRotatedPicPhysical(ScreenPlacement* scrPlace, float x, float y, float width, float height, float angle, vec4_t* color, Material* material)
{
    GameCall<void>(0x88FF0, scrPlace, x, y, width, height, angle, color, material);
}

double GPad_GetStick(int portIndex, int stick)
{
    return GameCall<double>(0x2FC614, portIndex, stick);
}


/* Custom */
bool keyPressed[32];

void R_ConvertColorToBytes(vec4_t color, unsigned char* bytes)
{
    for (int i = 0; i < 4; i++)
        bytes[i] = (unsigned char)(f_floorf(color.v[i] * 255));
}

float CalcLerpFraction(int startTime, int duration)
{
    if (duration < 1)
        return 1;

    int deltaTime = Sys_Milliseconds() - startTime;
    if (deltaTime >= duration)
        return 1;

    if (deltaTime < 1)
        return 0;

    return static_cast<float>(deltaTime) / static_cast<float>(duration);
}

float LerpFloat(float from, float to, float fraction)
{
    return from + (to - from) * fraction;
}

float ClampFloat(float value, float min, float max)
{
    return value < min ? min : value > max ? max : value;
}

int ClampInt(int value, int min, int max)
{
    return value < min ? min : value > max ? max : value;
}

float MaxFloat(float a1, float a2)
{
    return a1 > a2 ? a1 : a2;
}

float MinFloat(float a1, float a2)
{
    return a1 < a2 ? a1 : a2;
}

void RGBtoHSV(float r, float g, float b, float& h, float& s, float& v) 
{
    float cmax = MaxFloat(MaxFloat(r, g), b);
    float cmin = MinFloat(MinFloat(r, g), b);
    float delta = cmax - cmin;

    if (delta > 0)
    {
        if (cmax == r)
            h = 60 * (Game_Fmodf(((g - b) / delta), 6));
        else if (cmax == g)
            h = 60 * (((b - r) / delta) + 2);
        else if (cmax == b)
            h = 60 * (((r - g) / delta) + 4);

        if (cmax > 0)
            s = delta / cmax;
        else
            s = 0;

        v = cmax;
    }
    else 
    {
        h = 0;
        s = 0;
        v = cmax;
    }

    if (h < 0)
        h += 360;
}

void HSVtoRGB(float& r, float& g, float& b, float h, float s, float v) 
{
    float c = v * s; // Chroma
    float hPrime = Game_Fmodf(h / 60.0, 6);
    float x = c * (1 - __builtin_fabsf(Game_Fmodf(hPrime, 2) - 1));
    float m = v - c;

    if (0 <= hPrime && hPrime < 1) 
    {
        r = c;
        g = x;
        b = 0;
    }
    else if (1 <= hPrime && hPrime < 2) 
    {
        r = x;
        g = c;
        b = 0;
    }
    else if (2 <= hPrime && hPrime < 3) 
    {
        r = 0;
        g = c;
        b = x;
    }
    else if (3 <= hPrime && hPrime < 4) 
    {
        r = 0;
        g = x;
        b = c;
    }
    else if (4 <= hPrime && hPrime < 5) 
    {
        r = x;
        g = 0;
        b = c;
    }
    else if (5 <= hPrime && hPrime < 6) 
    {
        r = c;
        g = 0;
        b = x;
    }
    else 
    {
        r = 0;
        g = 0;
        b = 0;
    }

    r += m;
    g += m;
    b += m;
}


float GetFps()
{
    return f_floorf(1000 / *(float*)0xB8D1C4) + 0.5;
}