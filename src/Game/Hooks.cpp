
#include "Hooks.hpp"
#include "Functions.hpp"
#include "../Menu/Base.hpp"
#include "../Menu/Globals.hpp"
#include "../Utils/Memory.hpp"
#include <sysutil/sysutil_common.h>
#include <cell/gcm.h>


/*
    CG_DrawFps
        This function is a leftover from debug/qa builds of the game and is called once at the end 
        of a frame which is perfect to render our user interface.
*/

__declspec(naked, noinline) void CG_DrawFps_Stub(int localClient)
{
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __blr();
}

void CG_DrawFps_Hook(int localClient) 
{
    CG_DrawFps_Stub(localClient);

    g_Menu->RunControls();
    g_Menu->Render();
}


/*
    CL_GamepadButtonEventForPort
        This function is called when a button is pressed or released, by hooking it we can monitor 
        the buttons used and lock input of specific ones to avoid navigating the game user 
        interface at the same time as our menu.
*/

__declspec(naked, noinline) void CL_GamepadButtonEventForPort_Stub(int portIndex, int key, int buttonEvent, unsigned int time, int button)
{
    __nop(); 
    __nop(); 
    __nop(); 
    __nop(); 
    __nop();
    __nop();
    __nop();
    __blr();
}

void CL_GamepadButtonEventForPort_Hook(int portIndex, int key, int buttonEvent, unsigned int time, int button)
{
    // 0 : Released | 1 : Pressed | 2 : Needs update (this one is rarely used)
    keyPressed[key] = buttonEvent;

    if (g_Menu->opened)
    {
        switch (key)
        {
        case(KEY_CIRCLE):
        case(KEY_CROSS):
        case(KEY_L1):
        case(KEY_R1):
        case(KEY_L2):
        case(KEY_R2):
        case(KEY_DPAD_UP):
        case(KEY_DPAD_DOWN):
        case(KEY_DPAD_LEFT):
        case(KEY_DPAD_RIGHT):
            buttonEvent = 0;
            break;
        }
    }

    CL_GamepadButtonEventForPort_Stub(portIndex, key, buttonEvent, time, button);
}


/*
    CL_GamepadEvent
        Hooking this allow us to disable analog input, ie: when we are using the color picker.
*/

__declspec(naked, noinline) void CL_GamepadEvent_Stub(int portIndex, int physicalAxis, int value, int time)
{
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __blr();
}

void CL_GamepadEvent_Hook(int portIndex, int physicalAxis, int value, int time)
{
    if (g_Menu->opened && g_Menu->colorPickerRendered)
    {
        switch (physicalAxis)
        {
        case(2): // lx
        case(3): // ly
            value = 0;
            break;
        }
    }

    CL_GamepadEvent_Stub(portIndex, physicalAxis, value, time);
}


/*
    sysutil_callback
        By hooking this function we can avoid the pause menu from being opened when pressing 
        the PS button, allowing us to take screenshots while in-game.
*/

__declspec(naked, noinline) void sysutil_callback_Stub(unsigned long long status, unsigned long long param, void* userdata)
{
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __blr();
}

void sysutil_callback_Hook(unsigned long long status, unsigned long long param, void* userdata)
{
#ifdef _DEBUG
    // Stop game from being paused if we open the in-game xmb
    if (status == CELL_SYSUTIL_SYSTEM_MENU_OPEN)
        return;
#endif

    sysutil_callback_Stub(status, param, userdata);
}


/*
    R_AddCmdDrawUIQuadsInternal
        This hook allow us to recolor the elements of the LUA based user interface of the game.
*/

__declspec(naked, noinline) void R_AddCmdDrawUIQuadsInternal_Stub(Material* material, short r4, int r5, vec2_t* verts, unsigned char* color, int r8)
{
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __blr();
}

void R_AddCmdDrawUIQuadsInternal_Hook(Material* material, short r4, int r5, vec2_t* verts, unsigned char* color, int r8)
{
    static const char* materialsToRecolor[]
    {
        "sp_lockout_fogscroll",
        "sp_lockout_fogscrollthin",
        "sp_lockout_blur_fogscroll",
        "hud_horizontal_compass",
        "visor_left_bracket",
        "visor_right_bracket",
        "menu_visor_grid_top",
        "menu_visor_grid_bottom",
        "menu_visor_grid_left",
        "menu_visor_grid_right",
        "loadbar_background"
    };

    if (material && material->name && color)
    {
        for (int i = 0; i < 11; i++)
        {
            // Change the color pointer
            if (I_stricmp(material->name, materialsToRecolor[i]) == 0)
            {
                unsigned char customColor[4];
                R_ConvertColorToBytes((i % 2) == 0 ? g_Globals->config.settings.colorAccentPrimary : g_Globals->config.settings.colorAccentSecondary, customColor);

                color = customColor;
                break;
            }
        }
    }

    R_AddCmdDrawUIQuadsInternal_Stub(material, r4, r5, verts, color, r8);
}


/*
    R_RunCommandBufferDebug and RB_DrawLitCommandBuffer
        Those 2 are used to render 3d commands, by filtering the debug label passed to it we can
        render the view model as lines instead of filled polygons.
*/

void RB_SetPolygonMode(int mode)
{
    *(unsigned char*)(0x301010EF) = 0x28; // Invalidate a renderer value, allowing us to edit the polygon mode
    g_gcmContext.SetBackPolygonMode(mode);
}

__declspec(naked, noinline) void R_RunCommandBufferDebug_Stub(const void* data, void* cmdBuf, const char* label)
{
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __blr();
}

void R_RunCommandBufferDebug_Hook(const void* data, void* cmdBuf, const char* label)
{
    if (g_Globals->config.main.viewModelWireframe && (I_stricmp(label, "DEPTH HACK PREPASS") == 0))
        RB_SetPolygonMode(CELL_GCM_POLYGON_MODE_LINE);

    R_RunCommandBufferDebug_Stub(data, cmdBuf, label);
    RB_SetPolygonMode(CELL_GCM_POLYGON_MODE_FILL);
}

__declspec(naked, noinline) unsigned int RB_DrawLitCommandBuffer_Stub(const void* data, const void* viewInfo, unsigned char cmdBufType, unsigned int cmdBufCount, void(*drawFunction)(const void*, void*, int), int phase, const char* label, unsigned int drawList, int timer)
{
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __blr();
    return 0;
}

unsigned int RB_DrawLitCommandBuffer_Hook(const void* data, const void* viewInfo, unsigned char cmdBufType, unsigned int cmdBufCount, void(*drawFunction)(const void*, void*, int), int phase, const char* label, unsigned int drawList, int timer)
{
    if (g_Globals->config.main.viewModelWireframe && (I_stricmp(label, "DEPTH_HACK") == 0))
        RB_SetPolygonMode(CELL_GCM_POLYGON_MODE_LINE);

    unsigned int ret = RB_DrawLitCommandBuffer_Stub(data, viewInfo, cmdBufType, cmdBufCount, drawFunction, phase, label, drawList, timer);
    RB_SetPolygonMode(CELL_GCM_POLYGON_MODE_FILL);

    return ret;
}


__declspec(naked, noinline) void Scr_PlayerDamage_Stub(int self, int inflictor, int attacker, int damage, int dflags, int meansOfDeath, int iWeapon, vec3_t* vPoint, vec3_t* vDir, const int hitLoc, int timeOffset, unsigned short boneIndex)
{
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __blr();
}

void Scr_PlayerDamage_Hook(int self, int inflictor, int attacker, int damage, int dflags, int meansOfDeath, int iWeapon, vec3_t* vPoint, vec3_t* vDir, const int hitLoc, int timeOffset, unsigned short boneIndex)
{
    // Considering it's Singleplayer we don't need to check the entity number
    float damageScale = (100 - g_Globals->config.main.damageAttenuation) / 100;
    damage = (int)(static_cast<float>(damage) * damageScale + 0.5);

    Scr_PlayerDamage_Stub(self, inflictor, attacker, damage, dflags, meansOfDeath, iWeapon, vPoint, vDir, hitLoc, timeOffset, boneIndex);
}

__declspec(naked, noinline) int BG_AddAmmoToClip_Stub(int ps, int weapon, int amount)
{
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __blr();
    return 0;
}

int BG_AddAmmoToClip_Hook(int ps, int weapon, int amount)
{
    // Sustain ammo
    if (g_Globals->config.main.unlimitedAmmo)
        amount = ClampInt(amount, 0, 0x7fffffff);

    return BG_AddAmmoToClip_Stub(ps, weapon, amount);
}

__declspec(naked, noinline) void ClientThink_real_Stub(int ent, usercmd_s* ucmd)
{
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __blr();
}

void ClientThink_real_Hook(int ent, usercmd_s* ucmd)
{

    ClientThink_real_Stub(ent, ucmd);
}