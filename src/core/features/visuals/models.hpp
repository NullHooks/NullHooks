#pragma once 
#include "core/features/features.hpp"

/*
 * The files should be placed in:
 *   - C:\Program Files (x86)\Steam\steamapps\common\Counter-Strike Global Offensive
 *     (Assuming you are on windows and that's where your cs:go is installed)
 *
 * Models:
 *   - Pickaxe: https://gamebanana.com/mods/171920
 * 
 * Plath meanings:
 *     v_*.mdl         -> viewmodel path
 *     w_*.mdl         -> worldmodel path
 *     w_*_dropped.mdl -> dropped worldmodel path
 */

namespace models {
	inline const char* v_knife_path = NULL;			// "models/weapons/v_minecraft_pickaxe.mdl" | "models/weapons/caleon1/screwdriver/v_knife_screwdriver.mdl" | "models/weapons/eminem/bananabit/v_bananabit.mdl"
	inline const char* w_knife_path = NULL;			// "models/weapons/w_minecraft_pickaxe.mdl"
	inline const char* v_ak_path = "models/weapons/eminem/ethereal/v_ethereal.mdl";			// "models/weapons/eminem/ethereal/v_ethereal.mdl"
	inline const char* w_ak_path = NULL;			// "models/weapons/eminem/ethereal/w_ethereal.mdl"
	inline const char* v_awp_path = NULL;			// "models/weapons/eminem/dsr_50/v_dsr_50_2.mdl"
	inline const char* w_awp_path = NULL;			// "models/weapons/eminem/dsr_50/w_dsr_50_2.mdl"
	inline const char* w_awp_drop_path = NULL;		// "models/weapons/eminem/dsr_50/w_dsr_50_dropped.mdl"
	inline const char* v_ssg08_path = NULL;			// "models/weapons/eminem/crysis3_predator_bow/v_crysis3_predator_bow.mdl"
	inline const char* w_ssg08_path = NULL;			// "models/weapons/eminem/crysis3_predator_bow/w_crysis3_predator_bow.mdl"
	inline const char* w_ssg08_drop_path = NULL;	// "models/weapons/eminem/crysis3_predator_bow/w_crysis3_predator_bow_dropped.mdl"
	inline const char* player_path = NULL;			// Kinda broken
	inline const char* arms_path = NULL;
}