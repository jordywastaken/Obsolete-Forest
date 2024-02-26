
#pragma once

#include "../Base.hpp"
#include "../Globals.hpp"

namespace Tabs
{
    static void Main(Menu& menu)
    {
        menu.option("Damage attenuation")
            .slider(g_Globals->config.main.damageAttenuation, 0, 100, 1, 30, "%.0f%%");
        menu.option("Unlimited ammo")
            .toggle(g_Globals->config.main.unlimitedAmmo);
        menu.option("No clip (bound to ^BPS3ButtonTrigL^)")
            .toggle(g_Globals->config.main.noClip);
        menu.option("Camera");
        menu.option("View model");
    }
}