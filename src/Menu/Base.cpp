
#include "Base.hpp"
#include "Globals.hpp"
#include "Tabs/Main.hpp"
#include "Tabs/Settings.hpp"
#include "../Game/Helpers/UI.hpp"

Menu* g_Menu;

bool testToggle;
float testSliderFloat;
int testSliderInt;
float testColor[]{ 0.5, 0.5, 0.5, 1 };

Menu::Menu()
{
    rootMenu = [](Menu& menu)
    {
        menu.tab("Self", Tabs::Main);
        menu.tab("Entities", nullptr);
        menu.tab("World", nullptr);
        menu.tab("Loadout", nullptr);
        menu.tab("Settings", Tabs::Settings);
    };
}

void Menu::RunControls()
{
    if (ButtonReady())
    {
        if (!opened)
        {
            if (keyPressed[KEY_L1] && keyPressed[KEY_DPAD_UP])
                OnOpen();
        }
        else
        {
            if (keyPressed[KEY_L1] && keyPressed[KEY_DPAD_UP])
                OnClose();
            else if (keyPressed[KEY_CIRCLE])
                OnCancel();
            else if (keyPressed[KEY_DPAD_UP])
                OnScroll(-1);
            else if (keyPressed[KEY_DPAD_DOWN])
                OnScroll(1);
            else if (keyPressed[KEY_L2])
                OnTabChange(-1);
            else if (keyPressed[KEY_R2])
                OnTabChange(1);
        }
    }
}

void Menu::Render()
{
    assetMaterial = UI::RegisterMaterial("white");
    assetFont = UI::RegisterFont(fontPath[g_Globals->config.settings.font], false);

    tabCount = tabCurrentDrawn;
    tabCurrentDrawn = 0;

    optionCount = optionCurrentDrawn;
    optionCurrentDrawn = 0;

    // Setup colors
    colorAccentMixed =
    {
        LerpFloat(g_Globals->config.settings.colorAccentPrimary.r, g_Globals->config.settings.colorAccentSecondary.r, 0.5),
        LerpFloat(g_Globals->config.settings.colorAccentPrimary.g, g_Globals->config.settings.colorAccentSecondary.g, 0.5),
        LerpFloat(g_Globals->config.settings.colorAccentPrimary.b, g_Globals->config.settings.colorAccentSecondary.b, 0.5),
        LerpFloat(g_Globals->config.settings.colorAccentPrimary.a, g_Globals->config.settings.colorAccentSecondary.a, 0.5),
    };

    colorAccentPrimary_Dark = 
    {
        g_Globals->config.settings.colorAccentPrimary.r * 0.75f,
        g_Globals->config.settings.colorAccentPrimary.g * 0.75f,
        g_Globals->config.settings.colorAccentPrimary.b * 0.75f,
        g_Globals->config.settings.colorAccentPrimary.a
    };

    colorAccentSecondary_Dark =
    {
        g_Globals->config.settings.colorAccentSecondary.r * 0.75f,
        g_Globals->config.settings.colorAccentSecondary.g * 0.75f,
        g_Globals->config.settings.colorAccentSecondary.b * 0.75f,
        g_Globals->config.settings.colorAccentSecondary.a
    };

    colorGradient[0] = g_Globals->config.settings.colorAccentPrimary;
    colorGradient[1] = g_Globals->config.settings.colorAccentSecondary;
    colorGradient[2] = g_Globals->config.settings.colorAccentSecondary;
    colorGradient[3] = g_Globals->config.settings.colorAccentPrimary;

    colorGradient_Dark[0] = colorAccentPrimary_Dark;
    colorGradient_Dark[1] = colorAccentSecondary_Dark;
    colorGradient_Dark[2] = colorAccentSecondary_Dark;
    colorGradient_Dark[3] = colorAccentPrimary_Dark;

    colorGradient_Reversed[0] = colorGradient[1];
    colorGradient_Reversed[1] = colorGradient[0];
    colorGradient_Reversed[2] = colorGradient[3];
    colorGradient_Reversed[3] = colorGradient[2];

    DrawInstructions();
    DrawInfoboxes();

    if (g_Globals->config.settings.colorAccentFade)
    {
        g_Globals->colorAccentFadeTick += g_Globals->config.settings.colorAccentFadeSpeed;

        if (g_Globals->colorAccentFadeTick >= 360)
            g_Globals->colorAccentFadeTick = 0;

        float tickPrimary = g_Globals->colorAccentFadeTick;
        float tickSecondary = g_Globals->colorAccentFadeTick + g_Globals->config.settings.colorAccentFadeHueSpacing;

        // Make sure it is in range 0 - 360
        while (tickSecondary < 0)
            tickSecondary += 360;
        while (tickSecondary > 360)
            tickSecondary -= 360;

        HSVtoRGB(g_Globals->config.settings.colorAccentPrimary.r, g_Globals->config.settings.colorAccentPrimary.g, g_Globals->config.settings.colorAccentPrimary.b, tickPrimary, 1, 1);
        HSVtoRGB(g_Globals->config.settings.colorAccentSecondary.r, g_Globals->config.settings.colorAccentSecondary.g, g_Globals->config.settings.colorAccentSecondary.b, tickSecondary, 1, 1);
    }

    if (!opened && ButtonReady())
        return;

    // Setup coordinates and dimensions
    currentPos =
    {
        g_Globals->config.settings.posX,
        g_Globals->config.settings.posY + GetAnimationMenuPos()
    };

    currentSize = 
    {
        g_Globals->config.settings.sizeWidth,
        GetMenuHeight()
    };

    vec2_t tabSize
    {
        currentSize.x / tabCount,
        g_Globals->config.settings.sizeTab
    };

    // Menu outline and background
    UI::DrawRectangle(currentSize.x + 2, currentSize.y + 2, currentPos.x, currentPos.y - 1, g_Globals->config.settings.sizeOutline, colorGradient, UI_ANCHOR_TOP_CENTER, assetMaterial);
    UI::DrawFilledRectangle(currentSize.x, currentSize.y, currentPos.x, currentPos.y, g_Globals->config.settings.colorBackground, UI_ANCHOR_TOP_CENTER, assetMaterial);

    // Menu header
    UI::DrawTextWithGlow("Forest", g_Globals->config.settings.sizeHeader, currentPos.x, currentPos.y, colorWhite, colorAccentMixed, UI_ANCHOR_TOP_CENTER, assetFont);
    currentPos.y += g_Globals->config.settings.sizeHeader;
    UI::DrawFilledRectangle(currentSize.x, g_Globals->config.settings.sizeSeparator, currentPos.x, currentPos.y, g_Globals->config.settings.colorSeparator, UI_ANCHOR_TOP_CENTER, assetMaterial);
    currentPos.y += g_Globals->config.settings.sizeSeparator;

    // Tab background
    UI::DrawFilledRectangle(currentSize.x, tabSize.y, currentPos.x, currentPos.y, g_Globals->config.settings.colorTab, UI_ANCHOR_TOP_CENTER, assetMaterial);

    currentTabPos.x = currentPos.x - currentSize.x / 2;
    currentTabPos.y = currentPos.y;

    // Draw tab highlight
    if (submenuLevel == -1)
    {
        float tabHighlightPosX = GetAnimationTabPos();
        float tabColorFraction = tabHighlightPosX / (currentSize.x - tabSize.x);

        vec4_t tabColor
        {
            LerpFloat(g_Globals->config.settings.colorAccentPrimary.r, g_Globals->config.settings.colorAccentSecondary.r, tabColorFraction),
            LerpFloat(g_Globals->config.settings.colorAccentPrimary.g, g_Globals->config.settings.colorAccentSecondary.g, tabColorFraction),
            LerpFloat(g_Globals->config.settings.colorAccentPrimary.b, g_Globals->config.settings.colorAccentSecondary.b, tabColorFraction),
            LerpFloat(g_Globals->config.settings.colorAccentPrimary.a, g_Globals->config.settings.colorAccentSecondary.a, tabColorFraction),
        };

        vec4_t tabColor_Dark
        {
            tabColor.r * 0.75f,
            tabColor.g * 0.75f,
            tabColor.b * 0.75f,
            tabColor.a,
        };

        UI::DrawFilledRectangle(tabSize.x, tabSize.y, currentTabPos.x + tabHighlightPosX, currentPos.y, tabColor_Dark, UI_ANCHOR_TOP_LEFT, assetMaterial);
        currentPos.y += tabSize.y;
        UI::DrawFilledRectangle(tabSize.x, 2, currentTabPos.x + tabHighlightPosX, currentPos.y, tabColor, UI_ANCHOR_BOTTOM_LEFT, assetMaterial);
    }
    else
    {
        UI::DrawFilledRectangle(currentSize.x, tabSize.y, currentPos.x, currentPos.y, colorGradient_Dark, UI_ANCHOR_TOP_CENTER, assetMaterial);
        currentPos.y += tabSize.y;
        UI::DrawFilledRectangle(currentSize.x, 2, currentPos.x, currentPos.y, colorGradient, UI_ANCHOR_BOTTOM_CENTER, assetMaterial);
    }

    // Tab separator
    UI::DrawFilledRectangle(currentSize.x, g_Globals->config.settings.sizeSeparator, currentPos.x, currentPos.y, g_Globals->config.settings.colorSeparator, UI_ANCHOR_TOP_CENTER, assetMaterial);
    currentPos.y += g_Globals->config.settings.sizeSeparator + g_Globals->config.settings.sizeOptionStartY;

    // Draw option highlight
    if (optionCount > 0)
    {
        float optionHighlightY = currentPos.y + GetAnimationHighlightPos();
        UI::DrawFilledRectangle(currentSize.x, g_Globals->config.settings.sizeOption, currentPos.x, optionHighlightY, colorGradient_Dark, UI_ANCHOR_TOP_CENTER, assetMaterial);
        UI::DrawFilledRectangle(currentSize.x, 2, currentPos.x, optionHighlightY + g_Globals->config.settings.sizeOption, colorGradient, UI_ANCHOR_BOTTOM_CENTER, assetMaterial);
    }

    colorPickerRendered = false;

    if (rootMenu)
        rootMenu(*this);

    if (submenuLevel > -1)
        submenus[submenuLevel](*this);

    if (submenuToEnter)
        EnterSubmenu();
}

void Menu::OnOpen()
{
    opened = true;
    openTime = Sys_Milliseconds();
    ButtonWait(450);
}

void Menu::OnClose()
{
    opened = false;
    openTime = Sys_Milliseconds();
    ButtonWait(450);
}

void Menu::OnCancel()
{
    // We're not in a submenu
    if (submenuLevel == -1)
        OnClose();
    else
    {
        optionCurrent = submenuCurrentOption[submenuLevel];
        submenuLevel--;
        ButtonWait(200);
    }
}

void Menu::OnTabChange(int direction)
{
    // Don't go further if we are in a submenu
    if (submenuLevel > -1)
        return;

    int previousTab = tabCurrent;

    optionCurrent = 1;

    tabCurrent += direction;

    if (tabCurrent < 1)
        tabCurrent = tabCount;
    if (tabCurrent > tabCount)
        tabCurrent = 1;

    tabChangePreviousPos = (previousTab - 1) * (currentSize.x / tabCount);
    tabChangeTime = Sys_Milliseconds();
    ButtonWait(200);
}

void Menu::OnScroll(int direction)
{
    int previousOption = optionCurrent;

    optionCurrent += direction;

    if (optionCurrent < 1)
        optionCurrent = optionCount;
    if (optionCurrent > optionCount)
        optionCurrent = 1;

    optionChangePreviousPos = (previousOption - 1) * (g_Globals->config.settings.sizeOption + g_Globals->config.settings.sizeOptionSpacing);
    optionChangeTime = Sys_Milliseconds();
    ButtonWait(200);
}

void Menu::ButtonWait(int millis)
{
    buttonTick = Sys_Milliseconds();
    buttonWaitTime = millis;
}

bool Menu::ButtonReady()
{
    return Sys_Milliseconds() - buttonTick > buttonWaitTime;
}

void Menu::EnterSubmenu()
{
    if (submenuLevel < 9)
    {
        submenuLevel++;
        submenus[submenuLevel] = submenuToEnter;
        submenuCurrentOption[submenuLevel] = optionCurrent;
        optionCurrent = 1;
    }

    submenuToEnter = nullptr;
}

float Menu::GetAnimationMenuPos()
{
    if (opened)
        return LerpFloat(-720, 0, CalcLerpFraction(openTime, 400));

    return LerpFloat(0, -720, CalcLerpFraction(openTime, 400));
}

float Menu::GetAnimationTabPos()
{
    return LerpFloat(tabChangePreviousPos, (tabCurrent - 1) * (currentSize.x / tabCount), CalcLerpFraction(tabChangeTime, 175));
}

float Menu::GetAnimationHighlightPos()
{
    return LerpFloat(optionChangePreviousPos, (optionCurrent - 1) * (g_Globals->config.settings.sizeOption + g_Globals->config.settings.sizeOptionSpacing), CalcLerpFraction(optionChangeTime, 175));
}

float Menu::GetMenuHeight()
{
    float colorPickerHeight = colorPickerRendered ? colorPickerRectSize.y + colorPickerSliderSize + 70 : 0;
    float optionSpacing = optionCount > 0 ? (optionCount - 1) * g_Globals->config.settings.sizeOptionSpacing : 0;

    return g_Globals->config.settings.sizeHeader + g_Globals->config.settings.sizeTab + g_Globals->config.settings.sizeSeparator * 2 + g_Globals->config.settings.sizeOptionStartY + optionCount * g_Globals->config.settings.sizeOption + optionSpacing + colorPickerHeight;
}

void Menu::DrawTab(const char* name)
{
    tabCurrentDrawn++;

    float tabWidth = g_Globals->config.settings.sizeWidth / tabCount;
    float tabPosX = tabWidth * (tabCurrentDrawn - 1) + tabWidth / 2;

    UI::DrawText(name, g_Globals->config.settings.sizeTab - 4, currentTabPos.x + tabPosX, currentTabPos.y, colorWhite, UI_ANCHOR_TOP_CENTER, assetFont);
}

void Menu::DrawOption(const char* name)
{
    optionCurrentDrawn++;

    float baseX = currentTabPos.x + g_Globals->config.settings.sizeOptionStartX;
    float baseY = currentPos.y + (optionCurrentDrawn - 1) * (g_Globals->config.settings.sizeOption + g_Globals->config.settings.sizeOptionSpacing);

    UI::DrawText(name, g_Globals->config.settings.sizeOption - 2, baseX, baseY, colorWhite, UI_ANCHOR_TOP_LEFT, assetFont);
}

void Menu::DrawToggle(bool value)
{
    float baseX = currentPos.x + (currentSize.x / 2) - g_Globals->config.settings.sizeOptionStartX - g_Globals->config.settings.sizeBox / 2;
    float baseY = currentPos.y + (optionCurrentDrawn - 1) * (g_Globals->config.settings.sizeOption + g_Globals->config.settings.sizeOptionSpacing) + g_Globals->config.settings.sizeOption / 2;
    float size = g_Globals->config.settings.sizeBox;

    UI::DrawFilledRectangle(size, size, baseX, baseY, g_Globals->config.settings.colorSeparator, UI_ANCHOR_CENTER, assetMaterial);
    UI::DrawFilledRectangle(size - 2, size - 2, baseX, baseY, g_Globals->config.settings.colorBackground, UI_ANCHOR_CENTER, assetMaterial);

    if(value)
        UI::DrawFilledRectangle(size - 4, size - 4, baseX, baseY, g_Globals->config.settings.colorAccentPrimary, UI_ANCHOR_CENTER, assetMaterial);
}

void Menu::DrawSlider(const char* text, float progress, vec4_t color)
{
    float baseX = currentPos.x + (currentSize.x / 2) - g_Globals->config.settings.sizeOptionStartX - g_Globals->config.settings.sizeSliderWidth / 2;
    float baseY = currentPos.y + (optionCurrentDrawn - 1) * (g_Globals->config.settings.sizeOption + g_Globals->config.settings.sizeOptionSpacing) + g_Globals->config.settings.sizeOption / 2;
    float width = g_Globals->config.settings.sizeSliderWidth;
    float height = g_Globals->config.settings.sizeSliderHeight;

    // Slider frame
    UI::DrawFilledRectangle(width, height, baseX, baseY, g_Globals->config.settings.colorSeparator, UI_ANCHOR_CENTER, assetMaterial);
    UI::DrawFilledRectangle(width - 2, height - 2, baseX, baseY, g_Globals->config.settings.colorBackground, UI_ANCHOR_CENTER, assetMaterial);

    // Slider bar
    float progressX = baseX - width / 2 + width * progress - 2;
    UI::DrawFilledRectangle(width * progress - 4, height - 4, progressX, baseY, color, UI_ANCHOR_RIGHT, assetMaterial);

    // Slider cursor
    UI::DrawFilledRectangle(4, height + 3, progressX, baseY, g_Globals->config.settings.colorBackground, UI_ANCHOR_LEFT, assetMaterial);
    UI::DrawFilledRectangle(2, height + 1, progressX + 1, baseY, colorWhite, UI_ANCHOR_LEFT, assetMaterial);

    // Slider text
    UI::DrawText(text, g_Globals->config.settings.sizeOption - 2, baseX - width / 2 - 5, baseY - g_Globals->config.settings.sizeOption / 2, colorWhite, UI_ANCHOR_TOP_RIGHT, assetFont);
}

void Menu::DrawColorPicker(vec4_t& color)
{
    colorPickerRendered = true;
    
    float h, s, v;
    RGBtoHSV(color.r, color.g, color.b, h, s, v);

    float baseX = currentTabPos.x + g_Globals->config.settings.sizeOptionStartX;
    float baseY = currentPos.y + 5;

    colorPickerRectSize = 
    {
        (currentSize.x - 10) * 0.90f,
        (currentSize.x - 10) * 0.45f
    };
    colorPickerSliderSize = (currentSize.x - 10) - colorPickerRectSize.x - 10;

    colorPickerHue = 360 - h;
    colorPickerCursorPos.x = s * colorPickerRectSize.x;
    colorPickerCursorPos.y = (1 - v) * colorPickerRectSize.y;

    // Handle inputs
    colorPickerCursorPos.x = ClampFloat(colorPickerCursorPos.x + GPad_GetStick(0, 0x40000000) * 2.5, 1, colorPickerRectSize.x - 1);
    colorPickerCursorPos.y = ClampFloat(colorPickerCursorPos.y + -GPad_GetStick(0, 0x40000001) * 2.5, 1, colorPickerRectSize.y - 1);

    if (opened)
    {
        if (keyPressed[KEY_DPAD_LEFT])
            color.a += 0.01;
        if (keyPressed[KEY_DPAD_RIGHT])
            color.a -= 0.01;
        if (keyPressed[KEY_DPAD_UP])
            colorPickerHue -= 1;
        if (keyPressed[KEY_DPAD_DOWN])
            colorPickerHue += 1;
    }

    color.a = ClampFloat(color.a, 0, 1);
    colorPickerHue = ClampFloat(colorPickerHue, 1, 360);

    // Calculate and apply color values
    h = 360 - colorPickerHue;
    s = ClampFloat(colorPickerCursorPos.x / colorPickerRectSize.x, 0, 1);
    v = ClampFloat(1 - (colorPickerCursorPos.y / colorPickerRectSize.y), 0, 1);

    vec4_t currentColor;
    HSVtoRGB(color.r, color.g, color.b, h, s, v);
    HSVtoRGB(currentColor.r, currentColor.g, currentColor.b, h, 1, 1);
    currentColor.a = 1;

    // Draw the color picker
    vec4_t colorHueTable[7]
    {
        {1, 0, 0, 1},
        {1, 0, 1, 1},
        {0, 0, 1, 1},
        {0, 1, 1, 1},
        {0, 1, 0, 1},
        {1, 1, 0, 1},
        {1, 0, 0, 1},
    };

    vec4_t colorPickerRect[2][4]
    {
        {
            colorWhite,
            currentColor,
            currentColor,
            colorWhite
        },
        {
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            colorBlack,
            colorBlack
        }
    };

    // Draw picker rect
    UI::DrawFilledRectangle(colorPickerRectSize.x + 2, colorPickerRectSize.y + 2, baseX - 1, baseY - 1, g_Globals->config.settings.colorSeparator, UI_ANCHOR_TOP_LEFT, assetMaterial);
    UI::DrawFilledRectangle(colorPickerRectSize.x, colorPickerRectSize.y, baseX, baseY, colorPickerRect[0], UI_ANCHOR_TOP_LEFT, assetMaterial);
    UI::DrawFilledRectangle(colorPickerRectSize.x, colorPickerRectSize.y, baseX, baseY, colorPickerRect[1], UI_ANCHOR_TOP_LEFT, assetMaterial);

    // Draw picker cursor
    float pickerCursorSize = 3;
    UI::DrawFilledRectangle(pickerCursorSize + 2, pickerCursorSize + 2, baseX + colorPickerCursorPos.x, baseY + colorPickerCursorPos.y, g_Globals->config.settings.colorSeparator, UI_ANCHOR_CENTER, assetMaterial);
    UI::DrawFilledRectangle(pickerCursorSize, pickerCursorSize, baseX + colorPickerCursorPos.x, baseY + colorPickerCursorPos.y, colorWhite, UI_ANCHOR_CENTER, assetMaterial);

    // Draw hue slider
    UI::DrawFilledRectangle(colorPickerSliderSize + 2, colorPickerRectSize.y + 2, baseX + (currentSize.x - 10) + 1, baseY - 1, g_Globals->config.settings.colorSeparator, UI_ANCHOR_TOP_RIGHT, assetMaterial);
    for (int i = 0; i < 6; i++)
    {
        vec4_t colorHue[4]
        {
            colorHueTable[i],
            colorHueTable[i],
            colorHueTable[i + 1],
            colorHueTable[i + 1]
        };

        UI::DrawFilledRectangle(colorPickerSliderSize, colorPickerRectSize.y / 6, baseX + (currentSize.x - 10), baseY + (colorPickerRectSize.y / 6) * i, colorHue, UI_ANCHOR_TOP_RIGHT, assetMaterial);
    }
    float hueCursorPosY = (colorPickerHue / 360) * colorPickerRectSize.y;
    UI::DrawFilledRectangle(colorPickerSliderSize + 2, 3, baseX + (currentSize.x - 10) + 1, baseY + hueCursorPosY, g_Globals->config.settings.colorSeparator, UI_ANCHOR_CENTER_RIGHT, assetMaterial);
    UI::DrawFilledRectangle(colorPickerSliderSize, 1, baseX + (currentSize.x - 10), baseY + hueCursorPosY, colorWhite, UI_ANCHOR_CENTER_RIGHT, assetMaterial);

    // Draw alpha slider
    vec4_t colorAlpha[4]
    {
        currentColor,
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        currentColor
    };

    baseY += colorPickerRectSize.y + 10;

    UI::DrawRectangle((currentSize.x - 10), colorPickerSliderSize, baseX, baseY, 1, g_Globals->config.settings.colorSeparator, UI_ANCHOR_TOP_LEFT, assetMaterial);
    UI::DrawFilledRectangle((currentSize.x - 10), colorPickerSliderSize, baseX, baseY, colorAlpha, UI_ANCHOR_TOP_LEFT, assetMaterial);

    float alphaCursorPosX = (currentSize.x - 10) * (1 - color.a);
    UI::DrawFilledRectangle(3, colorPickerSliderSize + 2, baseX + alphaCursorPosX, baseY - 1, g_Globals->config.settings.colorSeparator, UI_ANCHOR_TOP_CENTER, assetMaterial);
    UI::DrawFilledRectangle(1, colorPickerSliderSize, baseX + alphaCursorPosX, baseY, colorWhite, UI_ANCHOR_TOP_CENTER, assetMaterial);

    // Draw color values
    float colorValuesSize = 20;
    baseY += colorPickerSliderSize + 5;

    GfxColor hexColor;
    R_ConvertColorToBytes(color, hexColor.array);

    UI::DrawRectangle((currentSize.x - 10), colorValuesSize, baseX, baseY, 1, g_Globals->config.settings.colorSeparator, UI_ANCHOR_TOP_LEFT, assetMaterial);
    UI::DrawText(va("HEX: #%02X%02X%02X%02X", hexColor.array[0], hexColor.array[1], hexColor.array[2], hexColor.array[3]), colorValuesSize, baseX + (currentSize.x - 10) / 2, baseY, colorWhite, UI_ANCHOR_TOP_CENTER, assetFont);
    
    baseY += colorValuesSize + 5;
    UI::DrawText(va("RGBA: %.0f, %.0f, %.0f, %.0f", color.r * 255, color.g * 255, color.b * 255, color.a * 255), colorValuesSize, baseX + 5, baseY, colorWhite, UI_ANCHOR_TOP_LEFT, assetFont);
    UI::DrawText(va("HSV: %.0f°, %.0f%%, %.0f%%", h, s * 100, v * 100), colorValuesSize, baseX + (currentSize.x - 10) - 5, baseY, colorWhite, UI_ANCHOR_TOP_RIGHT, assetFont);
}

void Menu::DrawSubmenu()
{
    float baseX = currentPos.x + currentSize.x / 2 - g_Globals->config.settings.sizeOptionStartX;
    float baseY = currentPos.y + (optionCurrentDrawn - 1) * (g_Globals->config.settings.sizeOption + g_Globals->config.settings.sizeOptionSpacing);

    UI::DrawText("->", g_Globals->config.settings.sizeOption - 2, baseX, baseY, colorWhite, UI_ANCHOR_TOP_RIGHT, assetFont);
}

void Menu::DrawInfobox(const char* text)
{
    float baseX = 1280 - g_Globals->config.settings.infoboxPosX;
    float baseY = g_Globals->config.settings.infoboxPosY + (g_Globals->config.settings.infoboxSizeOutline * 2 + g_Globals->config.settings.infoboxSizeHeight + g_Globals->config.settings.infoboxSizeSpacing) * infoboxCurrentDrawn;
    
    UI::DrawTextBox(text, g_Globals->config.settings.infoboxSizeHeight, baseX, baseY, g_Globals->config.settings.infoboxSizeOutline, colorWhite, g_Globals->config.settings.colorBackground, colorGradient_Reversed, UI_ANCHOR_TOP_RIGHT, assetFont, assetMaterial);

    infoboxCurrentDrawn++;
}

void Menu::DrawInfoboxes()
{
    infoboxCurrentDrawn = 0;

    if (g_Globals->config.settings.infoboxDrawWatermark)
    {
        DrawInfobox("Forest 1.00");
        infoboxCurrentDrawn++;
    }

    if (g_Globals->config.settings.infoboxDrawResolution)
        DrawInfobox(va("Resolution: %.0fx%0.f", UI::Internal::GetScreenWidth(), UI::Internal::GetScreenHeight()));
    
    if (g_Globals->config.settings.infoboxDrawFps)
        DrawInfobox(va("FPS: %i", (int)GetFps()));
}

void Menu::DrawInstruction(const char* text)
{
    float baseX = g_Globals->config.settings.infoboxPosX;
    float baseY = 720 - (g_Globals->config.settings.infoboxPosY + (g_Globals->config.settings.infoboxSizeOutline * 2 + g_Globals->config.settings.infoboxSizeHeight + g_Globals->config.settings.infoboxSizeSpacing) * instructionCurrentDrawn);

    UI::DrawTextBox(text, g_Globals->config.settings.infoboxSizeHeight, baseX, baseY, g_Globals->config.settings.infoboxSizeOutline, colorWhite, g_Globals->config.settings.colorBackground, colorGradient_Reversed, UI_ANCHOR_BOTTOM_LEFT, assetFont, assetMaterial);

    instructionCurrentDrawn++;
}

void Menu::DrawInstructions()
{
    instructionCurrentDrawn = 0;

    if (!g_Globals->config.settings.infoboxDrawInstructions)
        return;

    // Closed
    if (!opened)
    {
        DrawInstruction("Hold ^BPS3ButtonShoulderL^ & press ^BPS3ButtonDpadU^ to ^2open^7 the menu");
    }
    else
    {
        DrawInstruction("Hold ^BPS3ButtonShoulderL^ & press ^BPS3ButtonDpadU^ to ^1close^7 the menu");

        if (colorPickerRendered)
        {
            DrawInstruction("Use ^BPS3ButtonDpadU^ & ^BPS3ButtonDpadD^ to edit the color value");
            DrawInstruction("Use ^BPS3ButtonDpadL^ & ^BPS3ButtonDpadR^ to edit the color opacity");
            DrawInstruction("Use ^BPS3ButtonStickAnimatedL^ to move the color picker cursor");
        }
        else
        {
            DrawInstruction("Use ^BPS3ButtonDpadU^ & ^BPS3ButtonDpadD^ to scroll, ^BPS3ButtonX^ to select");

            if (submenuLevel == -1)
            {
                DrawInstruction("Use ^BPS3ButtonTrigL^ or ^BPS3ButtonTrigR^ to switch tabs");
            }
            else
            {
                DrawInstruction("Use ^BPS3ButtonCircle^ to go back");
            }
        }
    }
}

Menu& Menu::tab(const char* name, Menu::MenuContent tabContent)
{
    // If we're not in a submenu
    if(submenuLevel == -1)
    {
        DrawTab(name);

        // If the current tab is selected
        if (tabCurrent == tabCurrentDrawn)
        {
            if(tabContent)
                tabContent(*this);
        }
    }

    return *this;
}

Menu& Menu::title(const char* name)
{
    UI::DrawText(name, g_Globals->config.settings.sizeTab - 2, currentPos.x, currentTabPos.y, colorWhite, UI_ANCHOR_TOP_CENTER, assetFont);
    return *this;
}

Menu& Menu::option(const char* name)
{
    DrawOption(name);
    return *this;
}

Menu& Menu::action(void(*callback)())
{
    // Is hovered
    if (optionCurrent == optionCurrentDrawn)
    {
        if (ButtonReady() && keyPressed[KEY_CROSS])
        {
            if(callback)
                callback();

            ButtonWait(200);
        }
    }

    return *this;
}

Menu& Menu::toggle(bool& value, void(*callback)(bool))
{
    DrawToggle(value);

    // Is hovered
    if (optionCurrent == optionCurrentDrawn)
    {
        if (ButtonReady() && keyPressed[KEY_CROSS])
        {
            value ^= 1;

            if (callback)
                callback(value);

            ButtonWait(200);
        }
    }

    return *this;
}

Menu& Menu::slider(int& value, int min, int max, int step, int delay, const char* fmt)
{
    ProcessOptionItemControls<int>(value, min, max, step, delay);
    DrawSlider(va(fmt, value), static_cast<float>(value - min) / static_cast<float>(max - min), colorWhite);
    return *this;
}

Menu& Menu::slider(float& value, float min, float max, float step, int delay, const char* fmt)
{
    ProcessOptionItemControls<float>(value, min, max, step, delay);
    DrawSlider(va(fmt, value), (value - min) / (max - min), colorWhite);
    return *this;
}

Menu& Menu::color(float& value, vec4_t sliderColor)
{
    ProcessOptionItemControls<float>(value, 0, 1, 0.01, 0);
    DrawSlider(va("%.2f", value), value, sliderColor);
    return *this;
}

Menu& Menu::colorPicker(vec4_t& color)
{
    DrawColorPicker(color);
    return *this;
}

Menu& Menu::submenu(Menu::MenuContent submenuContent)
{
    DrawSubmenu();

    // Is hovered
    if (optionCurrent == optionCurrentDrawn)
    {
        if (ButtonReady() && keyPressed[KEY_CROSS])
        {
            if (submenuContent)
                submenuToEnter = submenuContent;

            ButtonWait(200);
        }
    }
    return *this;
}