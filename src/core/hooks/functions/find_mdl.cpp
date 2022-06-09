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
	std::string new_path = path;		// We will change (or not) the path in draw_models()

	visuals::models::draw_models(new_path);

	return findmdl::original(interfaces::mdl_cache, new_path.c_str());
}
