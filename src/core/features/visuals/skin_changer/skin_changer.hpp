#pragma once 
#include "core/features/features.hpp"

/*
 * More info: https://www.unknowncheats.me/wiki/Counter_Strike_Global_Offensive:Skin_Changer
 * Some skin ids: https://steamcommunity.com/sharedfiles/filedetails/?id=880595913
 * More skin ids: https://github.com/adamb70/CSGO-skin-ID-dumper/blob/master/item_index.txt
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
    float wear = 0.001f;			        // flFallbackWear		(Lower = Newer)
    std::string custom_name = "";		    // szCustomName
};

struct model {
    std::string viewmodel = "";
    std::string worldmodel = "";
    bool precache = false;
};

namespace skins {
    inline bool apply_skin(DWORD weapon_handle);
    void change_misc_models();
    void change_skins(client_frame_stage_t stage);
    void update_model(weapon_t* weapon);
    void fix_knife_animation(weapon_t* viewmodel_weapon, long& sequence); 
    bool custom_kill_icons(i_game_event* game_event);

    inline std::unordered_map<int, skin_info> custom_skins;
    void read_skins();
    inline void init_skin_config() {
        read_skins();
    }

    #pragma region SKIN_MAPS
    // Stores knife model names, no need to change
    inline std::unordered_map<int, model> default_models {
        { WEAPON_BAYONET,                   { "models/weapons/v_knife_bayonet.mdl",             "models/weapons/w_knife_bayonet.mdl",               false } },
        { WEAPON_KNIFE_M9_BAYONET,          { "models/weapons/v_knife_m9_bay.mdl",              "models/weapons/w_knife_m9_bay.mdl",                false } },
        { WEAPON_KNIFE_KARAMBIT,            { "models/weapons/v_knife_karam.mdl",               "models/weapons/w_knife_karam.mdl",                 false } },
        { WEAPON_KNIFE_SURVIVAL_BOWIE,      { "models/weapons/v_knife_survival_bowie.mdl",      "models/weapons/w_knife_survival_bowie.mdl",        false } },
        { WEAPON_KNIFE_BUTTERFLY,           { "models/weapons/v_knife_butterfly.mdl",           "models/weapons/w_knife_butterfly.mdl",             false } },
        { WEAPON_KNIFE_FALCHION,            { "models/weapons/v_knife_falchion_advanced.mdl",   "models/weapons/w_knife_falchion_advanced.mdl",     false } },
        { WEAPON_KNIFE_FLIP,                { "models/weapons/v_knife_flip.mdl",                "models/weapons/w_knife_flip.mdl",                  false } },
        { WEAPON_KNIFE_GUT,                 { "models/weapons/v_knife_gut.mdl",                 "models/weapons/w_knife_gut.mdl",                   false } },
        { WEAPON_KNIFE_TACTICAL,            { "models/weapons/v_knife_tactical.mdl",            "models/weapons/w_knife_tactical.mdl",              false } },      // Huntsman
        { WEAPON_KNIFE_PUSH,                { "models/weapons/v_knife_push.mdl",                "models/weapons/w_knife_push.mdl",                  false } },      
        { WEAPON_KNIFE_GYPSY_JACKKNIFE,     { "models/weapons/v_knife_gypsy_jackknife.mdl",     "models/weapons/w_knife_gypsy_jackknife.mdl",       false } },      
        { WEAPON_KNIFE_STILETTO,            { "models/weapons/v_knife_stiletto.mdl",            "models/weapons/w_knife_stiletto.mdl",              false } },      
        { WEAPON_KNIFE_WIDOWMAKER,          { "models/weapons/v_knife_widowmaker.mdl",          "models/weapons/w_knife_widowmaker.mdl",            false } },      // Talon
        { WEAPON_KNIFE_SKELETON,            { "models/weapons/v_knife_skeleton.mdl",            "models/weapons/w_knife_skeleton.mdl",              false } },      
        { WEAPON_KNIFE_URSUS,               { "models/weapons/v_knife_ursus.mdl",               "models/weapons/w_knife_ursus.mdl",                 false } },      
        { WEAPON_KNIFE_CSS,                 { "models/weapons/v_knife_css.mdl",                 "models/weapons/w_knife_css.mdl",                   false } }       // ???
    };
    // Custom models will get appended or overwriten
    inline std::unordered_map<int, model> custom_models = default_models;

    // Used for kill icons
    inline std::unordered_map<int, std::string> default_kill_icons {
        { WEAPON_KNIFE,                     "knife" },
        { WEAPON_KNIFE_T,                   "knife_t" },
        { WEAPON_BAYONET,                   "bayonet" },
        { WEAPON_KNIFE_M9_BAYONET,          "knife_m9_bayonet" },
        { WEAPON_KNIFE_KARAMBIT,            "knife_karambit" },
        { WEAPON_KNIFE_SURVIVAL_BOWIE,      "knife_survival_bowie" },
        { WEAPON_KNIFE_BUTTERFLY,           "knife_butterfly" },
        { WEAPON_KNIFE_FALCHION,            "knife_falchion" },
        { WEAPON_KNIFE_FLIP,                "knife_flip" },
        { WEAPON_KNIFE_GUT,                 "knife_gut" },
        { WEAPON_KNIFE_TACTICAL,            "knife_tactical" },          // Huntsman
        { WEAPON_KNIFE_PUSH,                "knife_push" },
        { WEAPON_KNIFE_GYPSY_JACKKNIFE,     "knife_gypsy_jackknife" },
        { WEAPON_KNIFE_STILETTO,            "knife_stiletto" },
        { WEAPON_KNIFE_WIDOWMAKER,          "knife_widowmaker" },        // Talon
        { WEAPON_KNIFE_SKELETON,            "knife_skeleton" },
        { WEAPON_KNIFE_URSUS,               "knife_ursus" },
        { WEAPON_KNIFE_CSS,                 "knife_css" }                // Classic
    };

    // Used for config reading in read_skins.cpp
    const std::unordered_map<std::string, int> qualities_map = {
        { "SKIN_QUALITY_NORMAL",        SKIN_QUALITY_NORMAL },
        { "SKIN_QUALITY_GENUINE",       SKIN_QUALITY_GENUINE },
        { "SKIN_QUALITY_VINTAGE",       SKIN_QUALITY_VINTAGE },
        { "SKIN_QUALITY_UNUSUAL",       SKIN_QUALITY_UNUSUAL },
        { "SKIN_QUALITY_COMMUNITY",     SKIN_QUALITY_COMMUNITY },
        { "SKIN_QUALITY_DEVELOPER",     SKIN_QUALITY_DEVELOPER },
        { "SKIN_QUALITY_SELF_MADE",     SKIN_QUALITY_SELF_MADE },
        { "SKIN_QUALITY_CUSTOMIZED",    SKIN_QUALITY_CUSTOMIZED },
        { "SKIN_QUALITY_STRANGE",       SKIN_QUALITY_STRANGE },
        { "SKIN_QUALITY_COMPLETED",     SKIN_QUALITY_COMPLETED },
        { "SKIN_QUALITY_TOURNAMENT",    SKIN_QUALITY_TOURNAMENT }
    };
    #pragma endregion
}
