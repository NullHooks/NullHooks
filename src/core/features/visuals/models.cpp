#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/features/visuals/models.hpp"

void visuals::custom_models::replace_model(char* path) {
    std::string model_path = "csgo/";
    
    // Viewmodels
    if (strstr(path, "v_")) {
        if (strstr(path, "knife_default_ct.mdl") || strstr(path, "knife_default_t.mdl")) {          // What model are wen changing
            //printf("%s", model_path.append(models::v_knife_path));
            //printf("%d", std::filesystem::exists(model_path.append(models::v_knife_path)));
            if (models::v_knife_path != NULL && std::filesystem::exists(model_path.append(models::v_knife_path))) {
                sprintf(path, models::v_knife_path);                                     // The model we want to replace it with
                custom_helpers::state_to_console_color("Models", "Replaced knife viewmodel");
            }
        } else if (strstr(path, "v_rif_ak47.mdl")) {
            if (models::v_ak_path != NULL && std::filesystem::exists(model_path.append(models::v_ak_path))) {
                sprintf(path, models::v_ak_path);
                custom_helpers::state_to_console_color("Models", "Replaced ak viewmodel");
            }
        } else if (strstr(path, "v_snip_awp.mdl")) {
            if (models::v_awp_path != NULL && std::filesystem::exists(model_path.append(models::v_awp_path))) {
                sprintf(path, models::v_awp_path);
                custom_helpers::state_to_console_color("Models", "Replaced awp viewmodel");
            }
        } else if (strstr(path, "v_snip_ssg08.mdl")) {
            if (models::v_ssg08_path != NULL && std::filesystem::exists(model_path.append(models::v_ssg08_path))) {
                sprintf(path, models::v_ssg08_path);
                custom_helpers::state_to_console_color("Models", "Replaced ssg08 viewmodel");
            }
        }
    // Worldmodels
    } else if (strstr(path, "w_")) {
        if (strstr(path, "knife_default_ct.mdl") || strstr(path, "knife_default_t.mdl")) {
            if (models::w_knife_path != NULL && std::filesystem::exists(model_path.append(models::w_knife_path))) {
                sprintf(path, models::w_knife_path);
                custom_helpers::state_to_console_color("Models", "Replaced knife worldmodel");
            }
        } else if (strstr(path, "w_rif_ak47.mdl")) {
            if (models::w_ak_path != NULL && std::filesystem::exists(model_path.append(models::w_ak_path))) {
                sprintf(path, models::w_ak_path);
                custom_helpers::state_to_console_color("Models", "Replaced ak worldmodel");
            }
        } else if (strstr(path, "w_snip_awp.mdl")) {
            if (models::w_awp_path != NULL && std::filesystem::exists(model_path.append(models::w_awp_path))) {
                sprintf(path, models::w_awp_path);
                custom_helpers::state_to_console_color("Models", "Replaced awp worldmodel");
            }
        } else if (strstr(path, "w_snip_awp_dropped.mdl")) {
            if (models::w_awp_drop_path != NULL && std::filesystem::exists(model_path.append(models::w_awp_drop_path))) {
                sprintf(path, models::w_awp_drop_path);
                custom_helpers::state_to_console_color("Models", "Replaced dropped awp worldmodel");
            }
        } else if (strstr(path, "w_snip_ssg08.mdl")) {
            if (models::w_ssg08_path != NULL && std::filesystem::exists(model_path.append(models::w_ssg08_path))) {
                sprintf(path, models::w_ssg08_path);
                custom_helpers::state_to_console_color("Models", "Replaced ssg08 worldmodel");
            }
        } else if (strstr(path, "w_snip_ssg08_dropped.mdl")) {
            if (models::w_ssg08_drop_path != NULL && std::filesystem::exists(model_path.append(models::w_ssg08_drop_path))) {
                sprintf(path, models::w_ssg08_drop_path);
                custom_helpers::state_to_console_color("Models", "Replaced dropped ssg08 worldmodel");
            }
        }
    // Other (players, arms, etc.)
    } else {
        if (strstr(path, "models/player")) {
            if (models::player_path != NULL && std::filesystem::exists(model_path.append(models::player_path))) {
                sprintf(path, models::player_path);
                custom_helpers::state_to_console_color("Models", "Replaced player model");
            }
        }
        
        if (strstr(path, "t_arms")) {
            if (models::arms_path != NULL && std::filesystem::exists(model_path.append(models::arms_path))) {
                sprintf(path, models::arms_path);
                custom_helpers::state_to_console_color("Models", "Replaced arms model");
            }
        }
    }
}
