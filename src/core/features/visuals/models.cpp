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

void visuals::models::replace_model(char* path) {
    std::string model_path = "csgo/";
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
        }
    } else if (strstr(path, "w_")) {
        if (strstr(path, "knife_default_ct.mdl") || strstr(path, "knife_default_t.mdl")) {
            if (variables::models::w_knife_path != NULL && std::filesystem::exists(model_path.append(variables::models::w_knife_path))) {
                sprintf(path, variables::models::w_knife_path);
                custom_helpers::state_to_console_color("Models", "Replaced knife worldmodel");
            }
        } else if (strstr(path, "v_rif_ak47.mdl")) {
            if (variables::models::w_ak_path != NULL && std::filesystem::exists(model_path.append(variables::models::w_ak_path))) {
                sprintf(path, variables::models::w_ak_path);
                custom_helpers::state_to_console_color("Models", "Replaced ak worldmodel");
            }
        }
    } else {
        if (strstr(path, "models/player")) {
            if (variables::models::player_path != NULL && std::filesystem::exists(model_path.append(variables::models::player_path))) {
                sprintf(path, variables::models::player_path);
                custom_helpers::state_to_console_color("Models", "Replaced player model");
            }
        }
        
        if (strstr(path, "t_arms_")) {
            if (variables::models::arms_path != NULL && std::filesystem::exists(model_path.append(variables::models::arms_path))) {
                sprintf(path, variables::models::arms_path);
                custom_helpers::state_to_console_color("Models", "Replaced arms model");
            }
        }
    }
}
