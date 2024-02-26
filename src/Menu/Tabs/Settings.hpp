
#pragma once

#include "../Base.hpp"
#include "../Globals.hpp"

namespace Tabs
{
    static void SettingsMenuLayout(Menu& menu)
    {
        menu.title("Menu layout");
        menu.option("Horizontal position")
            .slider(g_Globals->config.settings.posX, 0, 1280, 5, 0, "%.0f");
        menu.option("Vertical position")
            .slider(g_Globals->config.settings.posY, 0, 720, 5, 0, "%.0f");
        menu.option("Menu width")
            .slider(g_Globals->config.settings.sizeWidth, 300, 1280, 5, 0, "%.0f");
        menu.option("Header size")
            .slider(g_Globals->config.settings.sizeHeader, 10, 50, 1, 20, "%.0f");
        menu.option("Separator size")
            .slider(g_Globals->config.settings.sizeSeparator, 0, 10, 1, 20, "%.0f");
        menu.option("Outline thickness")
            .slider(g_Globals->config.settings.sizeOutline, 0, 10, 1, 20, "%.0f");
        menu.option("Tab size")
            .slider(g_Globals->config.settings.sizeTab, 10, 50, 1, 20, "%.0f");
        menu.option("Option size")
            .slider(g_Globals->config.settings.sizeOption, 10, 50, 1, 20, "%.0f");
        menu.option("Option spacing")
            .slider(g_Globals->config.settings.sizeOptionSpacing, 0, 15, 0.5, 20, "%.1f");
        menu.option("Option horizontal padding")
            .slider(g_Globals->config.settings.sizeOptionStartX, 0, 20, 0.5, 20, "%.1f");
        menu.option("Option vertical padding")
            .slider(g_Globals->config.settings.sizeOptionStartY, 0, 20, 0.5, 20, "%.1f");
        menu.option("Toggle size")
            .slider(g_Globals->config.settings.sizeBox, 0, 30, 0.5, 50, "%.1f");
        menu.option("Slider height")
            .slider(g_Globals->config.settings.sizeSliderHeight, 0, 30, 0.5, 50, "%.1f");
        menu.option("Slider width")
            .slider(g_Globals->config.settings.sizeSliderWidth, 0, 150, 1, 20, "%.0f");
    }

    static void SettingsMenuColors(Menu& menu)
    {
        menu.title("Menu colors");
        menu.option("Background opacity")
            .slider(g_Globals->config.settings.colorBackground.a, 0, 1, 0.01, 20);
        menu.option("Fade accent colors")
            .toggle(g_Globals->config.settings.colorAccentFade);
        menu.option("Fade speed")
            .slider(g_Globals->config.settings.colorAccentFadeSpeed, 0.1, 5, 0.1, 20, "%.1f");
        menu.option("Fade hue spacing")
            .slider(g_Globals->config.settings.colorAccentFadeHueSpacing, -180, 180, 2, 20, "%.0f");
        menu.option("Edit primary accent color")
            .submenu([](Menu& menu)
                {
                    menu.title("Editing: Primary accent color");
                    menu.colorPicker(g_Globals->config.settings.colorAccentPrimary);
                });
        menu.option("Edit secondary accent color")
            .submenu([](Menu& menu)
                {
                    menu.title("Editing: Secondary accent color");
                    menu.colorPicker(g_Globals->config.settings.colorAccentSecondary);
                });
    }

    static void SettingsInfoBoxes(Menu& menu)
    {
        menu.title("Info boxes");
        menu.option("Draw menu instructions")
            .toggle(g_Globals->config.settings.infoboxDrawInstructions);
        menu.option("Draw menu watermark")
            .toggle(g_Globals->config.settings.infoboxDrawWatermark);
        menu.option("Draw resolution")
            .toggle(g_Globals->config.settings.infoboxDrawResolution);
        menu.option("Draw FPS")
            .toggle(g_Globals->config.settings.infoboxDrawFps);
        menu.option("Horizontal padding")
            .slider(g_Globals->config.settings.infoboxPosX, 0, 100, 1, 20, "%.0f");
        menu.option("Vertical padding")
            .slider(g_Globals->config.settings.infoboxPosY, 0, 100, 1, 20, "%.0f");
        menu.option("Spacing")
            .slider(g_Globals->config.settings.infoboxSizeSpacing, 0, 15, 1, 20, "%.0f");
        menu.option("Height")
            .slider(g_Globals->config.settings.infoboxSizeHeight, 10, 30, 0.5, 20, "%.1f");
        menu.option("Outline thickness")
            .slider(g_Globals->config.settings.infoboxSizeOutline, 0, 10, 0.5, 20, "%.1f");
    }

    static void Settings(Menu& menu)
    {
        menu.option("Menu layout")
            .submenu(SettingsMenuLayout);
        menu.option("Menu colors")
            .submenu(SettingsMenuColors);
        menu.option("Info boxes")
            .submenu(SettingsInfoBoxes);
    }
}