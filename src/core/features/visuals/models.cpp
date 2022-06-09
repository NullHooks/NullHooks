#include "../features.hpp"

/*
 * The files should be placed in:
 *   - C:\Program Files (x86)\Steam\steamapps\common\Counter-Strike Global Offensive
 *     (Assuming you are on windows and that's where your cs:go is installed)
 *
 * Models:
 *   - Pickaxe: http://gamer-lab.com/home/hl-lab/var/mdl_source/2020/09_5/Iron_Pickaxe/__obj/p_iron_pickaxe.zip
 */

void visuals::models::draw_models(std::string &path) {
    if (strstr(path.c_str(), "knife_default_ct.mdl") || strstr(path.c_str(), "knife_default_t.mdl"))    // What model are wen changing
        path = "models/weapons/v_minecraft_pickaxe.mdl";                                // The model we want to replace it with
}
