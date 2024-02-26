
#include <sys/prx.h>
#include "Game/Functions.hpp"
#include "Game/Hooks.hpp"
#include "Menu/Base.hpp"
#include "Menu/Globals.hpp"
#include "Utils/Memory.hpp"

#pragma diag_suppress 77
SYS_MODULE_INFO(Forest, 0, 1, 1);
SYS_MODULE_START(Forest_Start);
SYS_MODULE_STOP(Forest_Stop);

/*
    Notes:
    - We use WII U and PC binary symbols to label functions on PS3
    - Opening binds of the menu will be L1 + DPAD-UP

*/

extern "C"
{
    int Forest_Start()
    {
        Game_Printf("Forest loaded\n");

        // Initialize our instances
        g_Globals = new Globals();
        g_Menu = new Menu();

        // Apply patches
        *(unsigned short*)(0x2FFE2C) = 0x4800; // Disable hang detection when we breakpoint

        // Install hooks
        HookFunctionStart(0x7FB60, *(unsigned int*)(CG_DrawFps_Hook), *(unsigned int*)(CG_DrawFps_Stub));
        HookFunctionStart(0x122890, *(unsigned int*)(CL_GamepadButtonEventForPort_Hook), *(unsigned int*)(CL_GamepadButtonEventForPort_Stub));
        HookFunctionStart(0x122404, *(unsigned int*)(CL_GamepadEvent_Hook), *(unsigned int*)(CL_GamepadEvent_Stub));
        HookFunctionStart(0x2FFBFC, *(unsigned int*)(sysutil_callback_Hook), *(unsigned int*)(sysutil_callback_Stub));
        HookFunctionStart(0x5B23D0, *(unsigned int*)(R_AddCmdDrawUIQuadsInternal_Hook), *(unsigned int*)(R_AddCmdDrawUIQuadsInternal_Stub));
        HookFunctionStart(0x5EDB78, *(unsigned int*)(R_RunCommandBufferDebug_Hook), *(unsigned int*)(R_RunCommandBufferDebug_Stub));
        HookFunctionStart(0x5EE0D4, *(unsigned int*)(RB_DrawLitCommandBuffer_Hook), *(unsigned int*)(RB_DrawLitCommandBuffer_Stub));
        HookFunctionStart(0x44780C, *(unsigned int*)(Scr_PlayerDamage_Hook), *(unsigned int*)(Scr_PlayerDamage_Stub));
        HookFunctionStart(0x56124, *(unsigned int*)(BG_AddAmmoToClip_Hook), *(unsigned int*)(BG_AddAmmoToClip_Stub));

        return 0;
    }

    int Forest_Stop()
    {
        Game_Printf("Forest unloaded\n");

        // Remove hooks
        UnHookFunctionStart(0x7FB60, *(unsigned int*)(CG_DrawFps_Stub));
        UnHookFunctionStart(0x122890, *(unsigned int*)(CL_GamepadButtonEventForPort_Stub));
        UnHookFunctionStart(0x122404, *(unsigned int*)(CL_GamepadEvent_Stub));
        UnHookFunctionStart(0x2FFBFC, *(unsigned int*)(sysutil_callback_Stub));
        UnHookFunctionStart(0x5B23D0, *(unsigned int*)(R_AddCmdDrawUIQuadsInternal_Stub));
        UnHookFunctionStart(0x5EDB78, *(unsigned int*)(R_RunCommandBufferDebug_Stub));
        UnHookFunctionStart(0x5EE0D4, *(unsigned int*)(RB_DrawLitCommandBuffer_Stub));
        UnHookFunctionStart(0x44780C, *(unsigned int*)(Scr_PlayerDamage_Stub));
        UnHookFunctionStart(0x56124, *(unsigned int*)(BG_AddAmmoToClip_Stub));

        // Destroy our instances
        delete g_Menu;
        delete g_Globals;

        return 0;
    }
}
