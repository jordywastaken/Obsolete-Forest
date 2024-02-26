
#include "UI.hpp"
#include <fastmath.h>

namespace UI
{
    namespace Internal
    {
        bool IsRectOnScreen(float x, float y, float width, float height)
        {
            if (x + width < 0 || x > 1280)
                return false;

            if (y + height < 0 || y > 720)
                return false;

            return true;
        }


        void ScaleToProject(float& x, float& y, float& width, float& height)
        {
            ScreenPlacement* scrPlace = ScrPlace_GetView(0);
            float xScale = scrPlace->realViewportSize.x / 1280;
            float yScale = scrPlace->realViewportSize.y / 720;

            x *= xScale;
            y *= yScale;
            width *= xScale;
            height *= yScale;
        }


        void SetPositionAnchor(float& x, float& y, float width, float height, int anchor)
        {
            switch (anchor & UI_ANCHOR_MASK_HORIZONTAL)
            {
            case(UI_ANCHOR_CENTER):
                x -= width * 0.5;
                break;
            case(UI_ANCHOR_RIGHT):
                x -= width;
                break;
            }

            switch (anchor & UI_ANCHOR_MASK_VERTICAL)
            {
            case(UI_ANCHOR_CENTER):
                y -= height * 0.5;
                break;
            case(UI_ANCHOR_BOTTOM):
                y -= height;
                break;
            }
        }


        float GetTextHeightFromScale(float height, Font* font)
        {
            if (!font)
                return 0.f;

            return height / static_cast<float>(font->height);
        }


        int GetLineCount(const char* text)
        {
            int count = 1;

            while (*text)
            {
                if (*text++ == '\n')
                    count++;
            }
            return count;
        }


        float GetScreenWidth()
        {
            return ScrPlace_GetView(0)->realViewportSize.x;
        }


        float GetScreenHeight()
        {
            return ScrPlace_GetView(0)->realViewportSize.y;
        }
    }


    Material* RegisterMaterial(const char* name)
    {
        XAsset asset;
        DB_FindXAssetHeader(&asset, ASSET_TYPE_MATERIAL, name, false, -1);
        return asset.material;
    }


    Font* RegisterFont(const char* name, bool addPath)
    {
        XAsset asset;
        DB_FindXAssetHeader(&asset, ASSET_TYPE_FONT, addPath ? R_GetFontPathFromName(name) : name, false, -1);
        return asset.font;
    }


    float GetTextWidth(const char* text, float lineHeight, Font* font)
    {
        if (!font)
            return 0.f;

        float textScale = Internal::GetTextHeightFromScale(lineHeight, font);
        float textWidth = R_TextWidth(0, text, 0x7fffffff, font);

        return textWidth * textScale;
    }


    void SetScissor(bool enabled, float x, float y, float width, float height, int anchor)
    {
        Internal::SetPositionAnchor(x, y, width, height, anchor);
        Internal::ScaleToProject(x, y, width, height);
        R_AddCmdSetScissorValuesInternal(enabled, (int)x, (int)y, (int)width, (int)height);
    }


    void DrawFilledRectangle(float width, float height, float x, float y, vec4_t colors[4], int anchor, Material* material)
    {
        if (!material)
        {
            if (material = RegisterMaterial("white"), !material)
                return;
        }

        Internal::SetPositionAnchor(x, y, width, height, anchor);
        if (!Internal::IsRectOnScreen(x, y, width, height))
            return;

        Internal::ScaleToProject(x, y, width, height);

        vec2_t verts[]
        {
            { x, y },
            { x + width, y },
            { x + width, y + height },
            { x, y + height }
        };

        vec2_t uvTable[]
        {
            { 0, 0 },
            { 1, 0 },
            { 1, 1 },
            { 0, 1 }
        };

        // Create the quad command
        GfxCmdDrawQuadList2D* cmd = (GfxCmdDrawQuadList2D*)(R_GetCommandBuffer(RC_DRAW_QUADLIST_2D, sizeof(GfxCmdDrawQuadList2D) + sizeof(GfxQuadVertex) * 4));

        if (cmd)
        {
            cmd->material = material;
            cmd->quadCount = 1;
            auto quadVertex = reinterpret_cast<GfxQuadVertex*>((int)(cmd) + sizeof(GfxCmdDrawQuadList2D));

            for (int i = 0; i < 4; i++)
            {
                R_ConvertColorToBytes(colors[i], quadVertex[i].color.array);
                quadVertex[i].xy.x = verts[i].x;
                quadVertex[i].xy.y = verts[i].y;
                quadVertex[i].st.x = uvTable[i].x;
                quadVertex[i].st.y = uvTable[i].y;
            }
        }
    }


    void DrawFilledRectangle(float width, float height, float x, float y, vec4_t color, int anchor, Material* material)
    {
        if (!color.a)
            return;

        if (!material)
        {
            if (material = RegisterMaterial("white"), !material)
                return;
        }

        Internal::SetPositionAnchor(x, y, width, height, anchor);
        if (!Internal::IsRectOnScreen(x, y, width, height))
            return;

        Internal::ScaleToProject(x, y, width, height);

        vec2_t verts[]
        {
            { x, y },
            { x + width, y },
            { x + width, y + height },
            { x, y + height }
        };

        // Create the quad command
        auto cmd = reinterpret_cast<GfxCmdDrawQuadPic*>(R_GetCommandBuffer(RC_DRAW_QUAD_PIC, sizeof(GfxCmdDrawQuadPic)));

        if (cmd)
        {
            cmd->material = material;
            cmd->w = 1.0;
            R_ConvertColorToBytes(color, cmd->color.array);

            for (int i = 0; i < 4; i++)
            {
                cmd->verts[i].x = verts[i].x;
                cmd->verts[i].y = verts[i].y;
            }
        }
    }
    

    void DrawRectangle(float width, float height, float x, float y, float borderSize, vec4_t colors[4], int anchor, Material* material)
    {
        vec4_t colorHorizontal[2][4]
        {
            { colors[0], colors[1], colors[1], colors[0] },
            { colors[3], colors[2], colors[2], colors[3] }
        };
        vec4_t colorVertical[2][4]
        {
            { colors[0], colors[0], colors[3], colors[3] },
            { colors[1], colors[1], colors[2], colors[2] }
        };

        Internal::SetPositionAnchor(x, y, width, height, anchor);
        DrawFilledRectangle(width + borderSize * 2, borderSize, x - borderSize, y, colorHorizontal[0], UI_ANCHOR_BOTTOM_LEFT, material); // Top
        DrawFilledRectangle(width + borderSize * 2, borderSize, x - borderSize, y + height, colorHorizontal[1], UI_ANCHOR_TOP_LEFT, material); // Bottom
        DrawFilledRectangle(borderSize, height, x, y, colorVertical[0], UI_ANCHOR_TOP_RIGHT, material); // Left
        DrawFilledRectangle(borderSize, height, x + width, y, colorVertical[1], UI_ANCHOR_TOP_LEFT, material); // Right
    }


    void DrawRectangle(float width, float height, float x, float y, float borderSize, vec4_t color, int anchor, Material* material)
    {
        if (!color.a)
            return;

        Internal::SetPositionAnchor(x, y, width, height, anchor);
        DrawFilledRectangle(width + borderSize * 2, borderSize, x - borderSize, y, color, UI_ANCHOR_BOTTOM_LEFT, material); // Top
        DrawFilledRectangle(width + borderSize * 2, borderSize, x - borderSize, y + height, color, UI_ANCHOR_TOP_LEFT, material); // Bottom
        DrawFilledRectangle(borderSize, height, x, y, color, UI_ANCHOR_TOP_RIGHT, material); // Left
        DrawFilledRectangle(borderSize, height, x + width, y, color, UI_ANCHOR_TOP_LEFT, material); // Right
    }


    void DrawLine(vec2_t from, vec2_t to, float thickness, vec4_t color, Material* material)
    {
        if (!color.a)
            return;

        if (!material)
        {
            if (material = RegisterMaterial("white"), !material)
                return;
        }

        float width = to.x - from.x;
        float height = to.y - from.y;
        float hypotenuse = f_sqrtf(width * width + height * height);
        float angle = Game_Atan2(height, width) * 57.2957795131; // Convert arctan result from radian to degree

        float x = from.x + ((width - hypotenuse) / 2);
        float y = from.y + ((height - thickness) / 2);

        if (!Internal::IsRectOnScreen(x, y, width, height))
            return;

        Internal::ScaleToProject(x, y, hypotenuse, thickness);
        CG_DrawRotatedPicPhysical(ScrPlace_GetView(0), x, y, hypotenuse, thickness, angle, &color, material);
    }


    GfxCmdDrawText2D* DrawText(const char* text, float lineHeight, float x, float y, vec4_t color, int anchor, Font* font)
    {
        if (!color.a)
            return nullptr;

        if (!font)
        {
            if (font = RegisterFont("normalFont"), !font)
                return nullptr;
        }

        // Text anchor is bottom left instead of top left
        y += lineHeight;

        float textWidth = GetTextWidth(text, lineHeight, font);
        vec2_t textScale = Internal::GetTextHeightFromScale(lineHeight, font);

        Internal::SetPositionAnchor(x, y, textWidth, lineHeight, anchor);
        if (!Internal::IsRectOnScreen(x, y - lineHeight, textWidth, lineHeight))
            return nullptr;

        Internal::ScaleToProject(x, y, textScale.x, textScale.y);
        return R_AddCmdDrawTextInternal(text, 0x7fffffff, font, x, y, textScale.x, textScale.y, 0.f, &color, 3);
    }


    GfxCmdDrawText2D* DrawTextWithGlow(const char* text, float lineHeight, float x, float y, vec4_t color, vec4_t glowColor, int anchor, Font* font)
    {
        auto cmd = DrawText(text, lineHeight, x, y, color, anchor, font);

        if (!cmd || !glowColor.a)
            return nullptr;

        glowColor.r *= 0.1;
        glowColor.g *= 0.1;
        glowColor.b *= 0.1;
        glowColor.a *= color.a;

        cmd->renderFlags |= TEXT_RENDERFLAG_GLOW | TEXT_RENDERFLAG_GLOW_FORCE_COLOR;
        R_ConvertColorToBytes(glowColor, cmd->glowForceColor.array);
        return cmd;
    }

    void DrawTextBox(const char* text, float lineHeight, float x, float y, float borderSize, vec4_t textColor, vec4_t backgroundColor, vec4_t borderColors[4], int anchor, Font* font, Material* material)
    {
        float boxPadding = lineHeight / 6;
        float boxWidth = GetTextWidth(text, lineHeight, font) + (boxPadding + borderSize) * 2;
        float boxHeight = lineHeight + borderSize * 2;

        x += borderSize;
        y += borderSize;

        Internal::SetPositionAnchor(x, y, boxWidth, boxHeight, anchor);

        boxWidth -= (borderSize * 2);

        DrawFilledRectangle(boxWidth, lineHeight, x, y, backgroundColor, UI_ANCHOR_TOP_LEFT, material); // Background
        DrawRectangle(boxWidth, lineHeight, x, y, borderSize, borderColors, UI_ANCHOR_TOP_LEFT, material); // Border
        DrawText(text, lineHeight, x + boxPadding, y, textColor, UI_ANCHOR_TOP_LEFT, font);
    }

    void DrawTextBox(const char* text, float lineHeight, float x, float y, float borderSize, vec4_t textColor, vec4_t backgroundColor, vec4_t borderColor, int anchor, Font* font, Material* material)
    {
        if (!borderColor.a)
            return;

        float boxPadding = lineHeight / 6;
        float boxWidth = GetTextWidth(text, lineHeight, font) + (boxPadding + borderSize) * 2;
        float boxHeight = lineHeight + borderSize * 2;

        x += borderSize;
        y += borderSize;

        Internal::SetPositionAnchor(x, y, boxWidth, boxHeight, anchor);

        boxWidth -= (borderSize * 2);

        DrawFilledRectangle(boxWidth, lineHeight, x, y, backgroundColor, UI_ANCHOR_TOP_LEFT, material); // Background
        DrawRectangle(boxWidth, lineHeight, x, y, borderSize, borderColor, UI_ANCHOR_TOP_LEFT, material); // Border
        DrawText(text, lineHeight, x + boxPadding, y, textColor, UI_ANCHOR_TOP_LEFT, font);
    }
}