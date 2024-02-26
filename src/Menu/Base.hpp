
#pragma once

#include "../Game/Functions.hpp"

class Menu
{
public:
    using MenuContent = void(*)(Menu&);

    Menu();

    void RunControls();
    void Render();

    Menu& tab(const char* name, MenuContent tabContent);
    Menu& title(const char* name);
    Menu& option(const char* name);
    Menu& action(void(*callback)());
    Menu& toggle(bool& value, void(*callback)(bool) = nullptr);
    Menu& slider(int& value, int min, int max, int step, int delay = 30, const char* fmt = "%i");
    Menu& slider(float& value, float min, float max, float step, int delay = 30, const char* fmt = "%.2f");
    Menu& color(float& value, vec4_t sliderColor);
    Menu& colorPicker(vec4_t& color);
    Menu& submenu(MenuContent submenuContent);

private:
    void OnOpen();
    void OnClose();
    void OnCancel();
    void OnTabChange(int direction);
    void OnScroll(int direction);

    void ButtonWait(int millis);
    bool ButtonReady();

    void EnterSubmenu();

    float GetAnimationMenuPos();
    float GetAnimationTabPos();
    float GetAnimationHighlightPos();
    float GetMenuHeight();

    void DrawTab(const char* name);
    void DrawOption(const char* name);
    void DrawToggle(bool value);
    void DrawSlider(const char* text, float progress, vec4_t color);
    void DrawColorPicker(vec4_t& color);
    void DrawSubmenu();

    void DrawInfobox(const char* text);
    void DrawInfoboxes();
    void DrawInstruction(const char* text);
    void DrawInstructions();

    template <typename T>
    void ProcessOptionItemControls(T& var, T min, T max, T step, int delay)
    {
        if (optionCurrent == optionCurrentDrawn && ButtonReady())
        {
            if (keyPressed[KEY_DPAD_LEFT])
            {
                if (var <= min)
                    var = min;
                else
                    var -= step;

                ButtonWait(delay);
            }

            if (var < min)
                var = min;

            if (keyPressed[KEY_DPAD_RIGHT])
            {
                if (var >= max)
                    var = max;
                else
                    var += step;

                ButtonWait(delay);
            }

            if (var > max)
                var = max;
        }
    }

public:
    bool opened{};
    int openTime{ -1 };

    // Tabs / Options / Info boxes
    int tabCurrent{ 1 };
    int tabCurrentDrawn{};
    int tabCount{};
    int tabChangeTime{ -1 };
    float tabChangePreviousPos{};

    int optionCurrent{ 1 };
    int optionCurrentDrawn{};
    int optionCount{};
    int optionChangeTime{ -1 };
    float optionChangePreviousPos{};

    int infoboxCurrentDrawn{};
    int instructionCurrentDrawn{};

    // Buttons
    int buttonTick{};
    int buttonWaitTime{};

    // Submenus
    MenuContent rootMenu{};
    MenuContent submenus[10]{};
    MenuContent submenuToEnter{};
    int submenuCurrentOption[10]{};
    int submenuLevel{ -1 };

    // Color picker
    bool colorPickerRendered{};
    vec2_t colorPickerCursorPos{};
    vec2_t colorPickerRectSize{};
    float colorPickerSliderSize{};
    float colorPickerHue{};

    // Assets
    Material* assetMaterial{};
    Font* assetFont{};

    // Colors
    vec4_t colorAccentMixed{};
    vec4_t colorAccentPrimary_Dark{};
    vec4_t colorAccentSecondary_Dark{};
    vec4_t colorGradient[4]{};
    vec4_t colorGradient_Dark[4]{};
    vec4_t colorGradient_Reversed[4]{};

    // Coordinates / Dimensions
    vec2_t currentPos{};
    vec2_t currentSize{};
    vec2_t currentTabPos{};
};

extern Menu* g_Menu;