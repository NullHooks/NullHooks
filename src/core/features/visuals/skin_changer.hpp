#pragma once 
#include "core/features/features.hpp"

// https://www.unknowncheats.me/wiki/Counter_Strike_Global_Offensive:Skin_Changer

struct skin_info {
    int paint_kit = 0;						// nFallbackPaintKit
    int seed = 0;							// nFallbackSeed
    int stattrack = -1;						// nFallbackStatTrak	(-1 = disabled, positive value is kill number)
    int quality = 4;						// iEntityQuality
    char* custom_name = nullptr;			// szCustomName			(Make sure you cast to char*)
    float wear = 0.1f;						// flFallbackWear		(Lower = Newer)
};

namespace skins {
    inline bool apply_skin(DWORD weapon_handle);
    void loop_skins(client_frame_stage_t stage);

    inline std::map<int, skin_info> custom_skins = {
    /*   Weapon id                      PaintKit, Seed, Stattrack, Quality, Name, Wear*/
        { WEAPON_GLOCK,                { 38, 0, 123, 4, nullptr, 0.5f } }
    };
}