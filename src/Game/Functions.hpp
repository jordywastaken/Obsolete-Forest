
#pragma once

#include "Structs.hpp"
#include <cell/gcm.h>

/* Game imports */
extern cell::Gcm::Inline::CellGcmContext& g_gcmContext;

extern double(*Game_Atan2)(double y, double x);
extern float(*Game_Fmodf)(float x, float y);
extern int(*Game_Printf)(const char* fmt, ...);
extern const char* (*va)(const char* fmt, ...);

void I_strncpyz(char* dest, const char* src, int destSize);
int I_stricmp(const char* str1, const char* str2);
int Sys_Milliseconds();

GfxCmdHeader* R_GetCommandBuffer(int renderCmd, int bytes);
GfxCmdDrawText2D* R_AddCmdDrawTextInternal(const char* text, int maxChars, Font* font, float x, float y, float xScale, float yScale, float rotation, vec4_t* color, int style);
void R_AddCmdSetScissorValuesInternal(bool enabled, int x, int y, int width, int height);
int R_TextWidth(int localClient, const char* text, int maxChars, Font* font);
const char* R_GetFontPathFromName(const char* name);

void DB_FindXAssetHeader(XAsset* asset, XAssetType type, const char* name, bool errorIfMissing, int waitTime);
ScreenPlacement* ScrPlace_GetView(int localClient);
void CG_DrawRotatedPicPhysical(ScreenPlacement* scrPlace, float x, float y, float width, float height, float angle, vec4_t* color, Material* material);

double GPad_GetStick(int portIndex, int stick);

/* Custom */
enum Keys_t
{
    KEY_CROSS = 1,
    KEY_CIRCLE = 2,
    KEY_SQUARE = 3,
    KEY_TRIANGLE = 4,
    KEY_L1 = 5,
    KEY_R1 = 6,
    KEY_START = 14,
    KEY_SELECT = 15,
    KEY_L3 = 16,
    KEY_R3 = 17,
    KEY_L2 = 18, 
    KEY_R2 = 19,
    KEY_DPAD_UP = 20,
    KEY_DPAD_DOWN = 21,
    KEY_DPAD_LEFT = 22,
    KEY_DPAD_RIGHT = 23,
};
extern bool keyPressed[32];

void R_ConvertColorToBytes(vec4_t color, unsigned char* bytes);

float CalcLerpFraction(int startTime, int duration);
float LerpFloat(float from, float to, float fraction);
float ClampFloat(float value, float min, float max);
int ClampInt(int value, int min, int max);

void RGBtoHSV(float r, float g, float b, float& h, float& s, float& v);
void HSVtoRGB(float& r, float& g, float& b, float h, float s, float v);

float GetFps();