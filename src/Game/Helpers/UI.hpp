
#pragma once

#include "../Functions.hpp"

enum UI_Anchor
{
    UI_ANCHOR_CENTER = 0,
    UI_ANCHOR_LEFT   = (1 << 0),
    UI_ANCHOR_RIGHT  = (1 << 1),
    UI_ANCHOR_TOP    = (1 << 2),
    UI_ANCHOR_BOTTOM = (1 << 3),

    UI_ANCHOR_TOP_LEFT      = (UI_ANCHOR_TOP | UI_ANCHOR_LEFT),
    UI_ANCHOR_TOP_RIGHT     = (UI_ANCHOR_TOP | UI_ANCHOR_RIGHT),
    UI_ANCHOR_TOP_CENTER    = (UI_ANCHOR_TOP | UI_ANCHOR_CENTER),
    UI_ANCHOR_BOTTOM_LEFT   = (UI_ANCHOR_BOTTOM | UI_ANCHOR_LEFT),
    UI_ANCHOR_BOTTOM_RIGHT  = (UI_ANCHOR_BOTTOM | UI_ANCHOR_RIGHT),
    UI_ANCHOR_BOTTOM_CENTER = (UI_ANCHOR_BOTTOM | UI_ANCHOR_CENTER),
    UI_ANCHOR_CENTER_LEFT   = (UI_ANCHOR_CENTER | UI_ANCHOR_LEFT),
    UI_ANCHOR_CENTER_RIGHT  = (UI_ANCHOR_CENTER | UI_ANCHOR_RIGHT),

    UI_ANCHOR_MASK_HORIZONTAL = (UI_ANCHOR_LEFT | UI_ANCHOR_RIGHT),
    UI_ANCHOR_MASK_VERTICAL   = (UI_ANCHOR_TOP  | UI_ANCHOR_BOTTOM)
};

namespace UI
{
    namespace Internal
    {
        bool IsRectOnScreen(float x, float y, float width, float height);
        void ScaleToProject(float& x, float& y, float& width, float& height);
        void SetPositionAnchor(float& x, float& y, float width, float height, int anchor);
        
        float GetTextHeightFromScale(float height, Font* font);
        int GetLineCount(const char* text);
        float GetScreenWidth();
        float GetScreenHeight();
    }

    Material* RegisterMaterial(const char* name);
    Font* RegisterFont(const char* name, bool addPath = true);

    float GetTextWidth(const char* text, float lineHeight, Font* font);
    void SetScissor(bool enabled, float x, float y, float width, float height, int anchor);

    void DrawFilledRectangle(float width, float height, float x, float y, vec4_t colors[4], int anchor, Material* material);
    void DrawFilledRectangle(float width, float height, float x, float y, vec4_t color, int anchor, Material* material);

    void DrawRectangle(float width, float height, float x, float y, float borderSize, vec4_t colors[4], int anchor, Material* material);
    void DrawRectangle(float width, float height, float x, float y, float borderSize, vec4_t color, int anchor, Material* material);

    void DrawLine(vec2_t from, vec2_t to, float thickness, vec4_t color, Material* material);

    GfxCmdDrawText2D* DrawText(const char* text, float lineHeight, float x, float y, vec4_t color, int anchor, Font* font);
    GfxCmdDrawText2D* DrawTextWithGlow(const char* text, float lineHeight, float x, float y, vec4_t color, vec4_t glowColor, int anchor, Font* font);

    void DrawTextBox(const char* text, float lineHeight, float x, float y, float borderSize, vec4_t textColor, vec4_t backgroundColor, vec4_t borderColor, int anchor, Font* font, Material* material);
    void DrawTextBox(const char* text, float lineHeight, float x, float y, float borderSize, vec4_t textColor, vec4_t backgroundColor, vec4_t borderColor[4], int anchor, Font* font, Material* material);
}
