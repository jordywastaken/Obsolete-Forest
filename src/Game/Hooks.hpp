
#pragma once

#include "Structs.hpp"

void CG_DrawFps_Stub(int localClient);
void CG_DrawFps_Hook(int localClient);

void CL_GamepadButtonEventForPort_Stub(int portIndex, int key, int buttonEvent, unsigned int time, int button);
void CL_GamepadButtonEventForPort_Hook(int portIndex, int key, int buttonEvent, unsigned int time, int button);

void CL_GamepadEvent_Stub(int portIndex, int physicalAxis, int value, int time);
void CL_GamepadEvent_Hook(int portIndex, int physicalAxis, int value, int time);

void sysutil_callback_Stub(unsigned long long status, unsigned long long param, void* userdata);
void sysutil_callback_Hook(unsigned long long status, unsigned long long param, void* userdata);

void R_AddCmdDrawUIQuadsInternal_Stub(Material* material, short r4, int r5, vec2_t* verts, unsigned char* color, int r8);
void R_AddCmdDrawUIQuadsInternal_Hook(Material* material, short r4, int r5, vec2_t* verts, unsigned char* color, int r8);

void R_RunCommandBufferDebug_Stub(const void* data, void* cmdBuf, const char* label);
void R_RunCommandBufferDebug_Hook(const void* data, void* cmdBuf, const char* label);

unsigned int RB_DrawLitCommandBuffer_Stub(const void* data, const void* viewInfo, unsigned char cmdBufType, unsigned int cmdBufCount, void(*drawFunction)(const void*, void*, int), int phase, const char* label, unsigned int drawList, int timer);
unsigned int RB_DrawLitCommandBuffer_Hook(const void* data, const void* viewInfo, unsigned char cmdBufType, unsigned int cmdBufCount, void(*drawFunction)(const void*, void*, int), int phase, const char* label, unsigned int drawList, int timer);

void Scr_PlayerDamage_Stub(int self, int inflictor, int attacker, int damage, int dflags, int meansOfDeath, int iWeapon, vec3_t* vPoint, vec3_t* vDir, const int hitLoc, int timeOffset, unsigned short boneIndex);
void Scr_PlayerDamage_Hook(int self, int inflictor, int attacker, int damage, int dflags, int meansOfDeath, int iWeapon, vec3_t* vPoint, vec3_t* vDir, const int hitLoc, int timeOffset, unsigned short boneIndex);

int BG_AddAmmoToClip_Stub(int ps, int weapon, int amount);
int BG_AddAmmoToClip_Hook(int ps, int weapon, int amount);