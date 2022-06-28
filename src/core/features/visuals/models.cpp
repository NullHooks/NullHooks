#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

/*
 * The files should be placed in:
 *   - C:\Program Files (x86)\Steam\steamapps\common\Counter-Strike Global Offensive
 *     (Assuming you are on windows and that's where your cs:go is installed)
 *
 * Models:
 *   - Pickaxe: https://gamebanana.com/mods/171920
 */

struct 

void visuals::models::replace_model(char* path) {
    std::string model_path = "csgo/";
    
    // Viewmodels
    if (strstr(path, "v_")) {
        if (strstr(path, "knife_default_ct.mdl") || strstr(path, "knife_default_t.mdl")) {          // What model are wen changing
            //printf("%s", model_path.append(variables::models::v_knife_path));
            //printf("%d", std::filesystem::exists(model_path.append(variables::models::v_knife_path)));
            if (variables::models::v_knife_path != NULL && std::filesystem::exists(model_path.append(variables::models::v_knife_path))) {
                sprintf(path, variables::models::v_knife_path);                                     // The model we want to replace it with
                custom_helpers::state_to_console_color("Models", "Replaced knife viewmodel");
            }
        } else if (strstr(path, "v_rif_ak47.mdl")) {
            if (variables::models::v_ak_path != NULL && std::filesystem::exists(model_path.append(variables::models::v_ak_path))) {
                sprintf(path, variables::models::v_ak_path);
                custom_helpers::state_to_console_color("Models", "Replaced ak viewmodel");
            }
        } else if (strstr(path, "v_snip_awp.mdl")) {
            if (variables::models::v_awp_path != NULL && std::filesystem::exists(model_path.append(variables::models::v_awp_path))) {
                sprintf(path, variables::models::v_awp_path);
                custom_helpers::state_to_console_color("Models", "Replaced awp viewmodel");
            }
        } else if (strstr(path, "v_snip_ssg08.mdl")) {
            if (variables::models::v_ssg08_path != NULL && std::filesystem::exists(model_path.append(variables::models::v_ssg08_path))) {
                sprintf(path, variables::models::v_ssg08_path);
                custom_helpers::state_to_console_color("Models", "Replaced ssg08 viewmodel");
            }
        }
    // Worldmodels
    } else if (strstr(path, "w_")) {
        if (strstr(path, "knife_default_ct.mdl") || strstr(path, "knife_default_t.mdl")) {
            if (variables::models::w_knife_path != NULL && std::filesystem::exists(model_path.append(variables::models::w_knife_path))) {
                sprintf(path, variables::models::w_knife_path);
                custom_helpers::state_to_console_color("Models", "Replaced knife worldmodel");
            }
        } else if (strstr(path, "w_rif_ak47.mdl")) {
            if (variables::models::w_ak_path != NULL && std::filesystem::exists(model_path.append(variables::models::w_ak_path))) {
                sprintf(path, variables::models::w_ak_path);
                custom_helpers::state_to_console_color("Models", "Replaced ak worldmodel");
            }
        } else if (strstr(path, "w_snip_awp.mdl")) {
            if (variables::models::w_awp_path != NULL && std::filesystem::exists(model_path.append(variables::models::w_awp_path))) {
                sprintf(path, variables::models::w_awp_path);
                custom_helpers::state_to_console_color("Models", "Replaced awp worldmodel");
            }
        } else if (strstr(path, "w_snip_awp_dropped.mdl")) {
            if (variables::models::w_awp_drop_path != NULL && std::filesystem::exists(model_path.append(variables::models::w_awp_drop_path))) {
                sprintf(path, variables::models::w_awp_drop_path);
                custom_helpers::state_to_console_color("Models", "Replaced dropped awp worldmodel");
            }
        } else if (strstr(path, "w_snip_ssg08.mdl")) {
            if (variables::models::w_ssg08_path != NULL && std::filesystem::exists(model_path.append(variables::models::w_ssg08_path))) {
                sprintf(path, variables::models::w_ssg08_path);
                custom_helpers::state_to_console_color("Models", "Replaced ssg08 worldmodel");
            }
        } else if (strstr(path, "w_snip_ssg08_dropped.mdl")) {
            if (variables::models::w_ssg08_drop_path != NULL && std::filesystem::exists(model_path.append(variables::models::w_ssg08_drop_path))) {
                sprintf(path, variables::models::w_ssg08_drop_path);
                custom_helpers::state_to_console_color("Models", "Replaced dropped ssg08 worldmodel");
            }
        }
    // Other (players, arms, etc.)
    } else {
        if (strstr(path, "models/player")) {
            if (variables::models::player_path != NULL && std::filesystem::exists(model_path.append(variables::models::player_path))) {
                sprintf(path, variables::models::player_path);
                custom_helpers::state_to_console_color("Models", "Replaced player model");
            }
        }
        
        if (strstr(path, "t_arms")) {
            if (variables::models::arms_path != NULL && std::filesystem::exists(model_path.append(variables::models::arms_path))) {
                sprintf(path, variables::models::arms_path);
                custom_helpers::state_to_console_color("Models", "Replaced arms model");
            }
        }
    }
}
