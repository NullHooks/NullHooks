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

enum {
	ARMS_MODEL_ID = 0,
	PLAYER_MODEL_ID
};

struct custom_model {
	const char* viewmodel;				// The viewmodel path			(v_*.mdl        )
	const char* worldmodel;				// The worldmodel path			(w_*.mdl        )
	const char* dropped_worldmodel;		// The dropped worldmodel path	(w_*_dropped.mdl)
};

namespace models {
	std::map<int, custom_model> weapon_models = {
	/*   Weapon id                           Viewmodel, Worldmodel, Dropped worldmodel */
		{ WEAPON_NONE,						{ NULL, NULL, NULL } },
		{ WEAPON_DEAGLE,					{ NULL, NULL, NULL } },
		{ WEAPON_ELITE,						{ NULL, NULL, NULL } },
		{ WEAPON_FIVESEVEN,					{ NULL, NULL, NULL } },
		{ WEAPON_GLOCK,						{ NULL, NULL, NULL } },
		{ WEAPON_AK47,						{ NULL, NULL, NULL } },
		{ WEAPON_AUG,						{ NULL, NULL, NULL } },
		{ WEAPON_AWP,						{ NULL, NULL, NULL } },
		{ WEAPON_FAMAS,						{ NULL, NULL, NULL } },
		{ WEAPON_G3SG1,						{ NULL, NULL, NULL } },
		{ WEAPON_GALILAR,					{ NULL, NULL, NULL } },
		{ WEAPON_M249,						{ NULL, NULL, NULL } },
		{ WEAPON_M4A1,						{ NULL, NULL, NULL } },
		{ WEAPON_MAC10,						{ NULL, NULL, NULL } },
		{ WEAPON_P90,						{ NULL, NULL, NULL } },
		{ WEAPON_MP5SD,						{ NULL, NULL, NULL } },
		{ WEAPON_UMP45,						{ NULL, NULL, NULL } },
		{ WEAPON_XM1014,					{ NULL, NULL, NULL } },
		{ WEAPON_BIZON,						{ NULL, NULL, NULL } },
		{ WEAPON_MAG7,						{ NULL, NULL, NULL } },
		{ WEAPON_NEGEV,						{ NULL, NULL, NULL } },
		{ WEAPON_SAWEDOFF,					{ NULL, NULL, NULL } },
		{ WEAPON_TEC9,						{ NULL, NULL, NULL } },
		{ WEAPON_TASER,						{ NULL, NULL, NULL } },
		{ WEAPON_HKP2000,					{ NULL, NULL, NULL } },
		{ WEAPON_MP7,						{ NULL, NULL, NULL } },
		{ WEAPON_MP9,						{ NULL, NULL, NULL } },
		{ WEAPON_NOVA,						{ NULL, NULL, NULL } },
		{ WEAPON_P250,						{ NULL, NULL, NULL } },
		{ WEAPON_SHIELD,					{ NULL, NULL, NULL } },
		{ WEAPON_SCAR20,					{ NULL, NULL, NULL } },
		{ WEAPON_SG556,						{ NULL, NULL, NULL } },
		{ WEAPON_SSG08,						{ NULL, NULL, NULL } },
		{ WEAPON_KNIFEGG,					{ NULL, NULL, NULL } },
		{ WEAPON_KNIFE,						{ NULL, NULL, NULL } },
		{ WEAPON_FLASHBANG,					{ NULL, NULL, NULL } },
		{ WEAPON_HEGRENADE,					{ NULL, NULL, NULL } },
		{ WEAPON_SMOKEGRENADE,				{ NULL, NULL, NULL } },
		{ WEAPON_MOLOTOV,					{ NULL, NULL, NULL } },
		{ WEAPON_DECOY,						{ NULL, NULL, NULL } },
		{ WEAPON_INCGRENADE,				{ NULL, NULL, NULL } },
		{ WEAPON_C4,						{ NULL, NULL, NULL } },
		{ WEAPON_HEALTHSHOT,				{ NULL, NULL, NULL } },
		{ WEAPON_KNIFE_T,					{ NULL, NULL, NULL } },
		{ WEAPON_M4A1_SILENCER,				{ NULL, NULL, NULL } },
		{ WEAPON_USP_SILENCER,				{ NULL, NULL, NULL } },
		{ WEAPON_CZ75A,						{ NULL, NULL, NULL } },
		{ WEAPON_REVOLVER,					{ NULL, NULL, NULL } },
		{ WEAPON_TAGRENADE,					{ NULL, NULL, NULL } },
		{ WEAPON_FISTS,						{ NULL, NULL, NULL } },
		{ WEAPON_BREACHCHARGE,				{ NULL, NULL, NULL } },
		{ WEAPON_TABLET,					{ NULL, NULL, NULL } },
		{ WEAPON_MELEE,						{ NULL, NULL, NULL } },
		{ WEAPON_AXE,						{ NULL, NULL, NULL } },
		{ WEAPON_HAMMER,					{ NULL, NULL, NULL } },
		{ WEAPON_SPANNER,					{ NULL, NULL, NULL } },
		{ WEAPON_KNIFE_GHOST,				{ NULL, NULL, NULL } },
		{ WEAPON_FIREBOMB,					{ NULL, NULL, NULL } },
		{ WEAPON_DIVERSION,					{ NULL, NULL, NULL } },
		{ WEAPON_FRAG_GRENADE,				{ NULL, NULL, NULL } },
		{ WEAPON_SNOWBALL,					{ NULL, NULL, NULL } },
		{ WEAPON_BUMPMINE,					{ NULL, NULL, NULL } },
		{ WEAPON_BAYONET,					{ NULL, NULL, NULL } },
		{ WEAPON_KNIFE_CSS,					{ NULL, NULL, NULL } },
		{ WEAPON_KNIFE_FLIP,				{ NULL, NULL, NULL } },
		{ WEAPON_KNIFE_GUT,					{ NULL, NULL, NULL } },
		{ WEAPON_KNIFE_KARAMBIT,			{ NULL, NULL, NULL } },
		{ WEAPON_KNIFE_M9_BAYONET,			{ NULL, NULL, NULL } },
		{ WEAPON_KNIFE_TACTICAL,			{ NULL, NULL, NULL } },
		{ WEAPON_KNIFE_FALCHION,			{ NULL, NULL, NULL } },
		{ WEAPON_KNIFE_SURVIVAL_BOWIE,		{ NULL, NULL, NULL } },
		{ WEAPON_KNIFE_BUTTERFLY,			{ NULL, NULL, NULL } },
		{ WEAPON_KNIFE_PUSH,				{ NULL, NULL, NULL } },
		{ WEAPON_KNIFE_URSUS,				{ NULL, NULL, NULL } },
		{ WEAPON_KNIFE_GYPSY_JACKKNIFE,		{ NULL, NULL, NULL } },
		{ WEAPON_KNIFE_STILETTO,			{ NULL, NULL, NULL } },
		{ WEAPON_KNIFE_WIDOWMAKER,			{ NULL, NULL, NULL } },
		{ GLOVE_STUDDED_BLOODHOUND,			{ NULL, NULL, NULL } },
		{ GLOVE_T_SIDE,						{ NULL, NULL, NULL } },
		{ GLOVE_CT_SIDE,					{ NULL, NULL, NULL } },
		{ GLOVE_SPORTY,						{ NULL, NULL, NULL } },
		{ GLOVE_SLICK,						{ NULL, NULL, NULL } },
		{ GLOVE_LEATHER_WRAP,				{ NULL, NULL, NULL } },
		{ GLOVE_MOTORCYCLE,					{ NULL, NULL, NULL } },
		{ GLOVE_SPECIALIST,					{ NULL, NULL, NULL } },
		{ GLOVE_HYDRA,						{ NULL, NULL, NULL } }
	};

	std::map<int, const char*> misc_models = {
	/*   Model id                           Model path */
		{ ARMS_MODEL_ID,					NULL },
		{ PLAYER_MODEL_ID,					NULL }	// Kinda broken
	};

	/*
	 * Some interesting models:
	 *     "models/weapons/v_minecraft_pickaxe.mdl"
	 *	   "models/weapons/caleon1/screwdriver/v_knife_screwdriver.mdl"
	 *     "models/weapons/eminem/bananabit/v_bananabit.mdl"
	 *     "models/weapons/eminem/ethereal/v_ethereal.mdl"
	 *     { "models/weapons/eminem/dsr_50/v_dsr_50_2.mdl", "models/weapons/eminem/dsr_50/w_dsr_50_2.mdl", "models/weapons/eminem/dsr_50/w_dsr_50_dropped.mdl" }
	 *     "models/weapons/eminem/crysis3_predator_bow/v_crysis3_predator_bow.mdl"
	 */
}