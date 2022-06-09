#include "../hooks.hpp"

/*
 * The files should be placed in:
 *   - C:\Program Files (x86)\Steam\steamapps\common\Counter-Strike Global Offensive
 *     (Assuming you are on windows and that's where your cs:go is installed)
 *
 * Models:
 *   - Pickaxe: http://gamer-lab.com/home/hl-lab/var/mdl_source/2020/09_5/Iron_Pickaxe/__obj/p_iron_pickaxe.zip
 */

unsigned long __stdcall hooks::findmdl::hook(const char* path) noexcept {
	if (strstr(path, "knife_default_ct.mdl"))                                                       // What model are wen changing
		return findmdl::original(interfaces::mdl_cache, "models/weapons/v_minecraft_pickaxe.mdl");  // The model we want to replace it with
	else if (strstr(path, "knife_default_t.mdl"))
		return findmdl::original(interfaces::mdl_cache, "models/weapons/v_minecraft_pickaxe.mdl");

	return findmdl::original(interfaces::mdl_cache, path);
}

