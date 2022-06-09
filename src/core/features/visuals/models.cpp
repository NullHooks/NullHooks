#include "../features.hpp"

void visuals::models::draw_models(std::string &path) {
    if (strstr(path.c_str(), "knife_default_ct.mdl") || strstr(path.c_str(), "knife_default_t.mdl"))    // What model are wen changing
        path = "models/weapons/v_minecraft_pickaxe.mdl";                                // The model we want to replace it with
}
