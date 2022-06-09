#include "../features.hpp"

void visuals::models::draw_models(const char* path) {
    if (strstr(path, "knife_default_ct.mdl") || strstr(path, "knife_default_t.mdl"))                    // What model are wen changing
        hooks::findmdl::original(interfaces::mdl_cache, "models/weapons/v_minecraft_pickaxe.mdl");      // The model we want to replace it with
}
