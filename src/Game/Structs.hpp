
#pragma once

#include <stdint.h>

// Math shit
struct vec2_t
{
    union
    {
        struct { float x, y; };
        float v[2];
    };

    vec2_t()
        : x(0), y(0)
    { }

    vec2_t(float value)
        : x(value), y(value)
    { }

    vec2_t(float* value)
        : x(value[0]), y(value[1])
    { }

    vec2_t(float x, float y)
        : x(x), y(y)
    { }
};

struct vec3_t
{
    union
    {
        struct { float x, y, z; };
        struct { float pitch, yaw, roll; };
        float v[3];
    };

    vec3_t()
        : x(0), y(0), z(0)
    { }

    vec3_t(float value)
        : x(value), y(value), z(value)
    { }

    vec3_t(float* value)
        : x(value[0]), y(value[1]), z(value[2])
    { }

    vec3_t(float x, float y, float z)
        : x(x), y(y), z(z)
    { }
};

struct vec4_t
{
    union
    {
        struct { float x, y, z, w; };
        struct { float r, g, b, a; };
        float v[4];
    };

    vec4_t()
        : x(0), y(0), z(0), w(0)
    { }

    vec4_t(float value)
        : x(value), y(value), z(value), w(value)
    { }

    vec4_t(float* value)
        : x(value[0]), y(value[1]), z(value[2]), w(value[3])
    { }

    vec4_t(float x, float y, float z, float w)
        : x(x), y(y), z(z), w(w)
    { }
};

// Asset shit
struct Material
{
    const char* name;
};

struct Font
{
    const char* name;
    int height;
};

enum XAssetType
{
    ASSET_TYPE_MATERIAL = 6,
    ASSET_TYPE_FONT = 22,
};

union XAsset
{
    Material* material;
    Font* font;
};

// UI shit
namespace LUI
{
    struct LUIElement
    {
        Material* material;
        Font* font;
        // ...
    };

    enum LUIAlignment
    {
        // ...
    };
}

struct ScreenPlacement
{
    vec2_t scaleVirtualToReal;
    vec2_t scaleVirtualToFull;
    vec2_t scaleRealToVirtual;
    vec2_t virtualViewableMin;
    vec2_t virtualViewableMax;
    vec2_t virtualTweakableMin;
    vec2_t virtualTweakableMax;
    vec2_t realViewportBase;
    vec2_t realViewportSize;
    vec2_t realViewportMid;
    vec2_t realViewableMin;
    vec2_t realViewableMax;
    vec2_t realTweakableMin;
    vec2_t realTweakableMax;
    vec2_t subScreen;
    float hudSplitscreenScale;
};

enum TextRenderFlags
{
    TEXT_RENDERFLAG_FORCEMONOSPACE = (1 << 0),
    TEXT_RENDERFLAG_CURSOR = (1 << 1),
    TEXT_RENDERFLAG_DROPSHADOW = (1 << 2),
    TEXT_RENDERFLAG_DROPSHADOW_EXTRA = (1 << 3),
    TEXT_RENDERFLAG_GLOW = (1 << 4),
    TEXT_RENDERFLAG_GLOW_FORCE_COLOR = (1 << 5),
    TEXT_RENDERFLAG_FX_DECODE = (1 << 6),
    TEXT_RENDERFLAG_PADDING = (1 << 7),
    TEXT_RENDERFLAG_SUBTITLETEXT = (1 << 8),
    TEXT_RENDERFLAG_CINEMATIC = (1 << 9),
    TEXT_RENDERFLAG_TYPEWRITER = (1 << 10),
    TEXT_RENDERFLAG_FADEIN = (1 << 11),
    TEXT_RENDERFLAG_COD7_TYPEWRITER = (1 << 12),
    TEXT_RENDERFLAG_POP_IN_WRITE = (1 << 13),
    TEXT_RENDERFLAG_REDACT = (1 << 14),
    TEXT_RENDERFLAG_JITTERSCALE = (1 << 15),
    TEXT_RENDERFLAG_COD7_DECODE = (1 << 16),
    TEXT_RENDERFLAG_AUTOWRAP = (1 << 17),
};

enum GfxRenderCommand
{
    RC_END_OF_LIST,
    RC_SET_CUSTOM_CONSTANT,
    RC_SET_MATERIAL_COLOR,
    RC_SAVE_SCREEN,
    RC_SAVE_SCREEN_SECTION,
    RC_CLEAR_SCREEN,
    RC_BEGIN_VIEW,
    RC_SET_VIEWPORT,
    RC_SET_SCISSOR,
    RC_RESOLVE_COMPOSITE,
    RC_STRETCH_PIC,
    RC_STRETCH_PIC_FLIP_ST,
    RC_STRETCH_PIC_ROTATE_XY,
    RC_STRETCH_PIC_ROTATE_ST,
    RC_DRAW_QUAD_PIC,
    RC_DRAW_FULL_SCREEN_COLORED_QUAD,
    RC_DRAW_TEXT_2D,
    RC_DRAW_TEXT_3D,
    RC_BLEND_SAVED_SCREEN_BLURRED,
    RC_BLEND_SAVED_SCREEN_FLASHED,
    RC_DRAW_POINTS,
    RC_DRAW_LINES,
    RC_DRAW_UI_QUADS,
    RC_DRAW_UI_QUADS_REPLACE_IMAGE,
    RC_DRAW_UI_TRIANGLES,
    RC_DRAW_TRIANGLES,
    RC_DRAW_QUADLIST_2D,
    RC_DRAW_EMBLEM_LAYER,
    RC_STRETCH_COMPOSITE,
    RC_PROJECTION_SET,
    RC_DRAW_FRAMED,
    RC_CONSTANT_SET
};

struct GfxCmdHeader
{
    unsigned short byteCount;
    unsigned char id;
    unsigned char ui3d;
};

union GfxColor
{
    unsigned int packed;
    unsigned char array[4];
};

struct GfxCmdDrawText2D
{
    GfxCmdHeader header;
    int type;
    float x;
    float y;
    float w;
    float h;
    float rotation;
    Font* font;
    float xScale;
    float yScale;
    GfxColor color;
    int maxChars;
    int renderFlags;
    int cursorPos;
    char cursorLetter;
    GfxColor glowForceColor;
    int fxBirthTime;
    int fxLetterTime;
    int fxDecayStartTime;
    int fxDecayDuration;
    int fxRedactDecayStartTime;
    int fxRedactDecayDuration;
    Material* fxMaterial;
    Material* fxMaterialGlow;
    float padding;
    char text[3];
};

struct GfxCmdDrawQuadPic
{
    GfxCmdHeader header;
    Material* material;
    vec2_t verts[4];
    float w;
    GfxColor color;
};

struct GfxCmdDrawQuadList2D
{ 
    GfxCmdHeader header; 
    Material* material; 
    int quadCount; 
};

struct GfxQuadVertex
{
    vec2_t xy;
    vec2_t st;
    GfxColor color;
};

// Entity/Player shit
struct usercmd_s
{
    int time;
    int buttonBits[2];
    int angles[3];

};