#pragma once 
#include "core/features/features.hpp"

// https://www.unknowncheats.me/wiki/Counter_Strike_Global_Offensive:Skin_Changer

enum {
    SKIN_QUALITY_NORMAL = 0,
    SKIN_QUALITY_GENUINE,
    SKIN_QUALITY_VINTAGE,
    SKIN_QUALITY_UNUSUAL,
    SKIN_QUALITY_COMMUNITY = 5,
    SKIN_QUALITY_DEVELOPER,
    SKIN_QUALITY_SELF_MADE,
    SKIN_QUALITY_CUSTOMIZED,
    SKIN_QUALITY_STRANGE,
    SKIN_QUALITY_COMPLETED,
    SKIN_QUALITY_TOURNAMENT = 12
};

struct skin_info {
    int item_definition_index = NULL;       // iItemDefinitionIndex
    int paint_kit = 0;						// nFallbackPaintKit
    int seed = 0;							// nFallbackSeed
    int stattrack = -1;						// nFallbackStatTrak	(-1 = disabled, positive value is kill number)
    int quality = 4;						// iEntityQuality       (https://www.unknowncheats.me/wiki/Counter_Strike_Global_Offensive:Skin_Changer#m_iEntityQuality)
    char* custom_name = nullptr;		    // szCustomName
    float wear = 0.001f;			        // flFallbackWear		(Lower = Newer)
};

namespace skins {
    inline bool apply_skin(DWORD weapon_handle);
    void loop_skins(client_frame_stage_t stage);

    inline std::unordered_map<int, skin_info> custom_skins;
    inline std::unordered_map<int, const char*> custom_models;      // Stores custom model names
    inline void init_skin_config() {
        // CT Knife
        custom_models[WEAPON_KNIFE] = "models/weapons/v_knife_m9_bay.mdl";
        custom_skins[WEAPON_KNIFE].item_definition_index = WEAPON_KNIFE_M9_BAYONET;
        custom_skins[WEAPON_KNIFE].paint_kit = 38;
        custom_skins[WEAPON_KNIFE].quality = SKIN_QUALITY_UNUSUAL;

        // T Knife
        custom_models[WEAPON_KNIFE_T] = "models/weapons/v_knife_butterfly.mdl";
        custom_skins[WEAPON_KNIFE_T].item_definition_index = WEAPON_KNIFE_BUTTERFLY;
        custom_skins[WEAPON_KNIFE_T].paint_kit = 33;
        custom_skins[WEAPON_KNIFE_T].quality = SKIN_QUALITY_UNUSUAL;

        custom_skins[WEAPON_GLOCK].paint_kit = 38;
        custom_skins[WEAPON_GLOCK].stattrack = 1337;

        custom_skins[WEAPON_AWP].paint_kit = 344;
        custom_skins[WEAPON_AWP].quality = SKIN_QUALITY_DEVELOPER;

        custom_skins[WEAPON_AK47].paint_kit = 675;

        custom_skins[WEAPON_DEAGLE].paint_kit = 37;
    }
}
