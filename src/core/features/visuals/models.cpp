#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/features/visuals/models.hpp"

void visuals::custom_models::replace_model(char* path) {
    const char* current_model = NULL;   // Will change depending on the path (func arg) in each conditional
    std::string model_path = "csgo/";
    
    // Viewmodels
    if (strstr(path, "v_")) {
        if (strstr(path, "knife_default_ct.mdl") || strstr(path, "knife_default_t.mdl")) {          // What model are wen changing
            current_model = models::weapon_models[WEAPON_KNIFE].viewmodel;
            if (current_model != NULL && std::filesystem::exists(model_path.append(current_model))) {
                sprintf(path, current_model);                                     // The model we want to replace it with
                helpers::console::state_to_console_color("Models", "Replaced knife viewmodel");
            }
        } else if (strstr(path, "v_rif_ak47.mdl")) {
            current_model = models::weapon_models[WEAPON_AK47].viewmodel;
            if (current_model != NULL && std::filesystem::exists(model_path.append(current_model))) {
                sprintf(path, current_model);
                helpers::console::state_to_console_color("Models", "Replaced ak47 viewmodel");
            }
        } else if (strstr(path, "v_snip_awp.mdl")) {
            current_model = models::weapon_models[WEAPON_AWP].viewmodel;
            if (current_model != NULL && std::filesystem::exists(model_path.append(current_model))) {
                sprintf(path, current_model);
                helpers::console::state_to_console_color("Models", "Replaced awp viewmodel");
            }
        } else if (strstr(path, "v_snip_ssg08.mdl")) {
            current_model = models::weapon_models[WEAPON_SSG08].viewmodel;
            if (current_model != NULL && std::filesystem::exists(model_path.append(current_model))) {
                sprintf(path, current_model);
                helpers::console::state_to_console_color("Models", "Replaced ssg08 viewmodel");
            }
        }
    // Worldmodels
    } else if (strstr(path, "w_")) {
        if (strstr(path, "knife_default_ct.mdl") || strstr(path, "knife_default_t.mdl")) {
            current_model = models::weapon_models[WEAPON_KNIFE].worldmodel;
            if (current_model != NULL && std::filesystem::exists(model_path.append(current_model))) {
                sprintf(path, current_model);
                helpers::console::state_to_console_color("Models", "Replaced knife worldmodel");
            }
        } else if (strstr(path, "w_rif_ak47.mdl")) {
            current_model = models::weapon_models[WEAPON_AK47].worldmodel;
            if (current_model != NULL && std::filesystem::exists(model_path.append(current_model))) {
                sprintf(path, current_model);
                helpers::console::state_to_console_color("Models", "Replaced ak47 worldmodel");
            }
        } else if (strstr(path, "w_rif_ak47_dropped.mdl")) {
            current_model = models::weapon_models[WEAPON_AK47].dropped_worldmodel;
            if (current_model != NULL && std::filesystem::exists(model_path.append(current_model))) {
                sprintf(path, current_model);
                helpers::console::state_to_console_color("Models", "Replaced dropped ak47 worldmodel");
            }
        } else if (strstr(path, "w_snip_awp.mdl")) {
            current_model = models::weapon_models[WEAPON_AWP].worldmodel;
            if (current_model != NULL && std::filesystem::exists(model_path.append(current_model))) {
                sprintf(path, current_model);
                helpers::console::state_to_console_color("Models", "Replaced awp worldmodel");
            }
        } else if (strstr(path, "w_snip_awp_dropped.mdl")) {
            current_model = models::weapon_models[WEAPON_AWP].dropped_worldmodel;
            if (current_model != NULL && std::filesystem::exists(model_path.append(current_model))) {
                sprintf(path, current_model);
                helpers::console::state_to_console_color("Models", "Replaced dropped awp worldmodel");
            }
        } else if (strstr(path, "w_snip_ssg08.mdl")) {
            current_model = models::weapon_models[WEAPON_SSG08].worldmodel;
            if (current_model != NULL && std::filesystem::exists(model_path.append(current_model))) {
                sprintf(path, current_model);
                helpers::console::state_to_console_color("Models", "Replaced ssg08 worldmodel");
            }
        } else if (strstr(path, "w_snip_ssg08_dropped.mdl")) {
            current_model = models::weapon_models[WEAPON_SSG08].dropped_worldmodel;
            if (current_model != NULL && std::filesystem::exists(model_path.append(current_model))) {
                sprintf(path, current_model);
                helpers::console::state_to_console_color("Models", "Replaced dropped ssg08 worldmodel");
            }
        }
    // Other (players, arms, etc.)
    } else {
        if (strstr(path, "models/player")) {
            current_model = models::misc_models[PLAYER_MODEL_ID];
            if (current_model != NULL && std::filesystem::exists(model_path.append(current_model))) {
                sprintf(path, current_model);
                helpers::console::state_to_console_color("Models", "Replaced player model");
            }
        }
        
        if (strstr(path, "t_arms")) {
            current_model = models::misc_models[ARMS_MODEL_ID];
            if (current_model != NULL && std::filesystem::exists(model_path.append(current_model))) {
                sprintf(path, current_model);
                helpers::console::state_to_console_color("Models", "Replaced arms model");
            }
        }
    }
}
