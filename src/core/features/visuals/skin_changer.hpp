#pragma once 
#include "core/features/features.hpp"

/*
 * More info: https://www.unknowncheats.me/wiki/Counter_Strike_Global_Offensive:Skin_Changer
 * 
 * First thing that the skin changer will do is change the weapons buy id if there is a different one in the map,
 * so if you want to change your knife, set the item_definition_index of your current knife (default for example)
 * to the new target knife, and then store your custom skin settings like paint or seed or whatever in that id's
 * index. Simple example:
 *     custom_skins[WEAPON_KNIFE].item_definition_index = WEAPON_KNIFE_M9_BAYONET;          // Change knife idx to bayonet
 *     custom_models[WEAPON_KNIFE_M9_BAYONET] = "models/weapons/v_knife_m9_bay.mdl";        // Specify bayonet model path
 *     custom_skins[WEAPON_KNIFE_M9_BAYONET].paint_kit = 417;                               // Change paint and seed of the NEW bayonet index
 *     custom_skins[WEAPON_KNIFE_M9_BAYONET].seed = 69;
 */

enum {
    // https://www.unknowncheats.me/wiki/Counter_Strike_Global_Offensive:Skin_Changer#m_iEntityQuality
    SKIN_QUALITY_NORMAL = 0,
    SKIN_QUALITY_GENUINE,           // "Genuine"
    SKIN_QUALITY_VINTAGE,           // "Vintage"
    SKIN_QUALITY_UNUSUAL,           // Knife star
    SKIN_QUALITY_COMMUNITY = 5,     // "Community"
    SKIN_QUALITY_DEVELOPER,         // "Valve"
    SKIN_QUALITY_SELF_MADE,         // "Prototype"
    SKIN_QUALITY_CUSTOMIZED,        // "Customized"
    SKIN_QUALITY_STRANGE,           // Stattrack
    SKIN_QUALITY_COMPLETED,         // "Completed"
    SKIN_QUALITY_TOURNAMENT = 12    // "Souvenir"
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
    void change_skins(client_frame_stage_t stage);
    void update_knife_model(weapon_t* weapon);
    void fix_knife_animation(weapon_t* viewmodel_weapon, long& sequence); 

    inline std::unordered_map<int, skin_info> custom_skins;
    inline void init_skin_config() {
        // CT Knife
        custom_skins[WEAPON_KNIFE].item_definition_index = WEAPON_KNIFE_M9_BAYONET;
        custom_skins[WEAPON_KNIFE_M9_BAYONET].paint_kit = 417;
        custom_skins[WEAPON_KNIFE_M9_BAYONET].seed = 69;
        custom_skins[WEAPON_KNIFE_M9_BAYONET].quality = SKIN_QUALITY_UNUSUAL;

        // T Knife
        custom_skins[WEAPON_KNIFE_T].item_definition_index = WEAPON_KNIFE_BUTTERFLY;
        custom_models[WEAPON_KNIFE_BUTTERFLY] = "models/weapons/v_knife_butterfly.mdl";
        custom_skins[WEAPON_KNIFE_BUTTERFLY].paint_kit = 417;
        custom_skins[WEAPON_KNIFE_BUTTERFLY].quality = SKIN_QUALITY_UNUSUAL;

        custom_skins[WEAPON_GLOCK].paint_kit = 38;
        custom_skins[WEAPON_GLOCK].stattrack = 1337;

        custom_skins[WEAPON_USP_SILENCER].paint_kit = 653;
        
        custom_skins[WEAPON_DEAGLE].paint_kit = 962;
        custom_skins[WEAPON_DEAGLE].quality = SKIN_QUALITY_DEVELOPER;

        custom_skins[WEAPON_AK47].paint_kit = 639;

        custom_skins[WEAPON_M4A1].paint_kit = 844;

        custom_skins[WEAPON_AWP].paint_kit = 344;
        custom_skins[WEAPON_AWP].quality = SKIN_QUALITY_TOURNAMENT;
    }

    // Stores knife model names, no need to change
    inline std::unordered_map<int, const char*> custom_models {
        { WEAPON_BAYONET,               "models/weapons/v_knife_bayonet.mdl" },
        { WEAPON_KNIFE_M9_BAYONET,      "models/weapons/v_knife_m9_bay.mdl" },
        { WEAPON_KNIFE_KARAMBIT,        "models/weapons/v_knife_karam.mdl" },
        { WEAPON_KNIFE_SURVIVAL_BOWIE,  "models/weapons/v_knife_survival_bowie.mdl" },
        { WEAPON_KNIFE_BUTTERFLY,       "models/weapons/v_knife_butterfly.mdl" },
        { WEAPON_KNIFE_FALCHION,        "models/weapons/v_knife_falchion_advanced.mdl" },
        { WEAPON_KNIFE_FLIP,            "models/weapons/v_knife_flip.mdl" },
        { WEAPON_KNIFE_GUT,             "models/weapons/v_knife_gut.mdl" },
        { WEAPON_KNIFE_TACTICAL,        "models/weapons/v_knife_tactical.mdl" },        // Huntsman
        { WEAPON_KNIFE_PUSH,            "models/weapons/v_knife_push.mdl" },
        { WEAPON_KNIFE_GYPSY_JACKKNIFE, "models/weapons/v_knife_gypsy_jackknife.mdl" },
        { WEAPON_KNIFE_STILETTO,        "models/weapons/v_knife_stiletto.mdl" },
        { WEAPON_KNIFE_WIDOWMAKER,      "models/weapons/v_knife_widowmaker.mdl" },      // Talon
        { WEAPON_KNIFE_URSUS,           "models/weapons/v_knife_ursus.mdl" },
        { WEAPON_KNIFE_CSS,             "models/weapons/v_knife_css.mdl" }              // ???
    };
    // Stores knife worldmodel names, no need to change
    inline std::unordered_map<int, const char*> custom_wolrdmodels {
        { WEAPON_BAYONET,               "models/weapons/w_knife_bayonet.mdl" },
        { WEAPON_KNIFE_M9_BAYONET,      "models/weapons/w_knife_m9_bay.mdl" },
        { WEAPON_KNIFE_KARAMBIT,        "models/weapons/w_knife_karam.mdl" },
        { WEAPON_KNIFE_SURVIVAL_BOWIE,  "models/weapons/w_knife_survival_bowie.mdl" },
        { WEAPON_KNIFE_BUTTERFLY,       "models/weapons/w_knife_butterfly.mdl" },
        { WEAPON_KNIFE_FALCHION,        "models/weapons/w_knife_falchion_advanced.mdl" },
        { WEAPON_KNIFE_FLIP,            "models/weapons/w_knife_flip.mdl" },
        { WEAPON_KNIFE_GUT,             "models/weapons/w_knife_gut.mdl" },
        { WEAPON_KNIFE_TACTICAL,        "models/weapons/w_knife_tactical.mdl" },        // Huntsman
        { WEAPON_KNIFE_PUSH,            "models/weapons/w_knife_push.mdl" },
        { WEAPON_KNIFE_GYPSY_JACKKNIFE, "models/weapons/w_knife_gypsy_jackknife.mdl" },
        { WEAPON_KNIFE_STILETTO,        "models/weapons/w_knife_stiletto.mdl" },
        { WEAPON_KNIFE_WIDOWMAKER,      "models/weapons/w_knife_widowmaker.mdl" },      // Talon
        { WEAPON_KNIFE_URSUS,           "models/weapons/w_knife_ursus.mdl" },
        { WEAPON_KNIFE_CSS,             "models/weapons/w_knife_css.mdl" }              // ???
    };
}
