#pragma once
#include <array>
#include "dependencies/math/math.hpp"
#include "dependencies/interfaces/interfaces.hpp"
#include "source-sdk/structs/animstate.hpp"
#include "dependencies/utilities/utilities.hpp"
#include "dependencies/utilities/netvars/netvars.hpp"

enum data_update_type_t {
	DATA_UPDATE_CREATED = 0,
	DATA_UPDATE_DATATABLE_CHANGED,
};

enum cs_weapon_type {
	WEAPONTYPE_KNIFE = 0,
	WEAPONTYPE_PISTOL,
	WEAPONTYPE_SUBMACHINEGUN,
	WEAPONTYPE_RIFLE,
	WEAPONTYPE_SHOTGUN,
	WEAPONTYPE_SNIPER_RIFLE,
	WEAPONTYPE_MACHINEGUN,
	WEAPONTYPE_C4,
	WEAPONTYPE_PLACEHOLDER,
	WEAPONTYPE_GRENADE,
	WEAPONTYPE_UNKNOWN
};

enum observer_modes {
	OBS_MODE_NONE = 0,	// not in spectator mode
	OBS_MODE_DEATHCAM,	// special mode for death cam animation
	OBS_MODE_FREEZECAM,	// zooms to a target, and freeze-frames on them
	OBS_MODE_FIXED,		// view from a fixed camera position
	OBS_MODE_IN_EYE,	// follow a player in first person view
	OBS_MODE_CHASE,		// follow a player in third person view
	OBS_MODE_ROAMING,	// free roaming

	NUM_OBSERVER_MODES,
};


enum client_frame_stage_t {
	FRAME_UNDEFINED = -1,			// (haven't run any frames yet)
	FRAME_START,

	// A network packet is being recieved
	FRAME_NET_UPDATE_START,
	// Data has been received and we're going to start calling PostDataUpdate
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	// Data has been received and we've called PostDataUpdate on all data recipients
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	// We've received all packets, we can now do interpolation, prediction, etc..
	FRAME_NET_UPDATE_END,

	// We're about to start rendering the scene
	FRAME_RENDER_START,
	// We've finished rendering the scene.
	FRAME_RENDER_END
};

enum move_type {
	movetype_none = 0,
	movetype_isometric,
	movetype_walk,
	movetype_step,
	movetype_fly,
	movetype_flygravity,
	movetype_vphysics,
	movetype_push,
	movetype_noclip,
	movetype_ladder,
	movetype_observer,
	movetype_custom,
	movetype_last = movetype_custom,
	movetype_max_bits = 4,
	max_movetype
};

enum entity_flags {
	fl_onground = (1 << 0),
	fl_ducking = (1 << 1),
	fl_waterjump = (1 << 2),
	fl_ontrain = (1 << 3),
	fl_inrain = (1 << 4),
	fl_frozen = (1 << 5),
	fl_atcontrols = (1 << 6),
	fl_client = (1 << 7),
	fl_fakeclient = (1 << 8),
	fl_inwater = (1 << 9),
	fl_fly = (1 << 10),
	fl_swim = (1 << 11),
	fl_conveyor = (1 << 12),
	fl_npc = (1 << 13),
	fl_godmode = (1 << 14),
	fl_notarget = (1 << 15),
	fl_aimtarget = (1 << 16),
	fl_partialground = (1 << 17),
	fl_staticprop = (1 << 18),
	fl_graphed = (1 << 19),
	fl_grenade = (1 << 20),
	fl_stepmovement = (1 << 21),
	fl_donttouch = (1 << 22),
	fl_basevelocity = (1 << 23),
	fl_worldbrush = (1 << 24),
	fl_object = (1 << 25),
	fl_killme = (1 << 26),
	fl_onfire = (1 << 27),
	fl_dissolving = (1 << 28),
	fl_transragdoll = (1 << 29),
	fl_unblockable_by_player = (1 << 30)
};
enum item_definition_indexes {
	WEAPON_NONE = 0,
	WEAPON_DEAGLE,
	WEAPON_ELITE,
	WEAPON_FIVESEVEN,
	WEAPON_GLOCK,
	WEAPON_AK47 = 7,
	WEAPON_AUG,
	WEAPON_AWP,
	WEAPON_FAMAS,
	WEAPON_G3SG1,
	WEAPON_GALILAR = 13,
	WEAPON_M249,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10,
	WEAPON_P90 = 19,
	WEAPON_MP5SD = 23,
	WEAPON_UMP45,
	WEAPON_XM1014,
	WEAPON_BIZON,
	WEAPON_MAG7,
	WEAPON_NEGEV,
	WEAPON_SAWEDOFF,
	WEAPON_TEC9,
	WEAPON_TASER,
	WEAPON_HKP2000,
	WEAPON_MP7,
	WEAPON_MP9,
	WEAPON_NOVA,
	WEAPON_P250,
	WEAPON_SHIELD,
	WEAPON_SCAR20,
	WEAPON_SG556,
	WEAPON_SSG08,
	WEAPON_KNIFEGG,
	WEAPON_KNIFE,
	WEAPON_FLASHBANG,
	WEAPON_HEGRENADE,
	WEAPON_SMOKEGRENADE,
	WEAPON_MOLOTOV,
	WEAPON_DECOY,
	WEAPON_INCGRENADE,
	WEAPON_C4,
	WEAPON_HEALTHSHOT = 57,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER,
	WEAPON_USP_SILENCER,
	WEAPON_CZ75A = 63,
	WEAPON_REVOLVER,
	WEAPON_TAGRENADE = 68,
	WEAPON_FISTS,
	WEAPON_BREACHCHARGE,
	WEAPON_TABLET = 72,
	WEAPON_MELEE = 74,
	WEAPON_AXE,
	WEAPON_HAMMER,
	WEAPON_SPANNER = 78,
	WEAPON_KNIFE_GHOST = 80,
	WEAPON_FIREBOMB,
	WEAPON_DIVERSION,
	WEAPON_FRAG_GRENADE,
	WEAPON_SNOWBALL,
	WEAPON_BUMPMINE,
	WEAPON_BAYONET = 500,
	WEAPON_KNIFE_CSS = 503,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT,
	WEAPON_KNIFE_KARAMBIT,
	WEAPON_KNIFE_M9_BAYONET,
	WEAPON_KNIFE_TACTICAL,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY,
	WEAPON_KNIFE_PUSH,
	WEAPON_KNIFE_PARACORD,
	WEAPON_KNIFE_SURVIVAL,
	WEAPON_KNIFE_URSUS = 519,
	WEAPON_KNIFE_GYPSY_JACKKNIFE,
	WEAPON_KNIFE_NOMAD,
	WEAPON_KNIFE_STILETTO = 522,
	WEAPON_KNIFE_WIDOWMAKER,
	WEAPON_KNIFE_SKELETON = 525,
	GLOVE_STUDDED_BLOODHOUND = 5027,
	GLOVE_T_SIDE = 5028,
	GLOVE_CT_SIDE = 5029,
	GLOVE_SPORTY = 5030,
	GLOVE_SLICK = 5031,
	GLOVE_LEATHER_WRAP = 5032,
	GLOVE_MOTORCYCLE = 5033,
	GLOVE_SPECIALIST = 5034,
	GLOVE_HYDRA = 5035
};

// Misc models
enum {
	LOCAL_PLAYER = 10000,
	PLAYER_ALLY,
	PLAYER_ENEMY,
	ARMS
};

// Array with all the items and names used for iteration and configs.
// Used in read_skins.cpp
const std::unordered_map<std::string, int> all_item_definition_indexes = {
	{ "WEAPON_NONE",					WEAPON_NONE },
	{ "WEAPON_DEAGLE",					WEAPON_DEAGLE },
	{ "WEAPON_ELITE",					WEAPON_ELITE },
	{ "WEAPON_FIVESEVEN",				WEAPON_FIVESEVEN },
	{ "WEAPON_GLOCK",					WEAPON_GLOCK },
	{ "WEAPON_AK47",					WEAPON_AK47 },
	{ "WEAPON_AUG",						WEAPON_AUG },
	{ "WEAPON_AWP",						WEAPON_AWP },
	{ "WEAPON_FAMAS",					WEAPON_FAMAS },
	{ "WEAPON_G3SG1",					WEAPON_G3SG1 },
	{ "WEAPON_GALILAR",					WEAPON_GALILAR },
	{ "WEAPON_M249",					WEAPON_M249 },
	{ "WEAPON_M4A1",					WEAPON_M4A1 },
	{ "WEAPON_MAC10",					WEAPON_MAC10 },
	{ "WEAPON_P90",						WEAPON_P90 },
	{ "WEAPON_MP5SD",					WEAPON_MP5SD },
	{ "WEAPON_UMP45",					WEAPON_UMP45 },
	{ "WEAPON_XM1014",					WEAPON_XM1014 },
	{ "WEAPON_BIZON",					WEAPON_BIZON },
	{ "WEAPON_MAG7",					WEAPON_MAG7 },
	{ "WEAPON_NEGEV",					WEAPON_NEGEV },
	{ "WEAPON_SAWEDOFF",				WEAPON_SAWEDOFF },
	{ "WEAPON_TEC9",					WEAPON_TEC9 },
	{ "WEAPON_TASER",					WEAPON_TASER },
	{ "WEAPON_HKP2000",					WEAPON_HKP2000 },
	{ "WEAPON_MP7",						WEAPON_MP7 },
	{ "WEAPON_MP9",						WEAPON_MP9 },
	{ "WEAPON_NOVA",					WEAPON_NOVA },
	{ "WEAPON_P250",					WEAPON_P250 },
	{ "WEAPON_SHIELD",					WEAPON_SHIELD },
	{ "WEAPON_SCAR20",					WEAPON_SCAR20 },
	{ "WEAPON_SG556",					WEAPON_SG556 },
	{ "WEAPON_SSG08",					WEAPON_SSG08 },
	{ "WEAPON_KNIFEGG",					WEAPON_KNIFEGG },
	{ "WEAPON_KNIFE",					WEAPON_KNIFE },
	{ "WEAPON_FLASHBANG",				WEAPON_FLASHBANG },
	{ "WEAPON_HEGRENADE",				WEAPON_HEGRENADE },
	{ "WEAPON_SMOKEGRENADE",			WEAPON_SMOKEGRENADE },
	{ "WEAPON_MOLOTOV",					WEAPON_MOLOTOV },
	{ "WEAPON_DECOY",					WEAPON_DECOY },
	{ "WEAPON_INCGRENADE",				WEAPON_INCGRENADE },
	{ "WEAPON_C4",						WEAPON_C4 },
	{ "WEAPON_HEALTHSHOT",				WEAPON_HEALTHSHOT },
	{ "WEAPON_KNIFE_T",					WEAPON_KNIFE_T },
	{ "WEAPON_M4A1_SILENCER",			WEAPON_M4A1_SILENCER },
	{ "WEAPON_USP_SILENCER",			WEAPON_USP_SILENCER },
	{ "WEAPON_CZ75A",					WEAPON_CZ75A },
	{ "WEAPON_REVOLVER",				WEAPON_REVOLVER },
	{ "WEAPON_TAGRENADE",				WEAPON_TAGRENADE },
	{ "WEAPON_FISTS",					WEAPON_FISTS },
	{ "WEAPON_BREACHCHARGE",			WEAPON_BREACHCHARGE },
	{ "WEAPON_TABLET",					WEAPON_TABLET },
	{ "WEAPON_MELEE",					WEAPON_MELEE },
	{ "WEAPON_AXE",						WEAPON_AXE },
	{ "WEAPON_HAMMER",					WEAPON_HAMMER },
	{ "WEAPON_SPANNER",					WEAPON_SPANNER },
	{ "WEAPON_KNIFE_GHOST",				WEAPON_KNIFE_GHOST },
	{ "WEAPON_FIREBOMB",				WEAPON_FIREBOMB },
	{ "WEAPON_DIVERSION",				WEAPON_DIVERSION },
	{ "WEAPON_FRAG_GRENADE",			WEAPON_FRAG_GRENADE },
	{ "WEAPON_SNOWBALL",				WEAPON_SNOWBALL },
	{ "WEAPON_BUMPMINE",				WEAPON_BUMPMINE },
	{ "WEAPON_BAYONET",					WEAPON_BAYONET },
	{ "WEAPON_KNIFE_CSS",				WEAPON_KNIFE_CSS },
	{ "WEAPON_KNIFE_FLIP",				WEAPON_KNIFE_FLIP },
	{ "WEAPON_KNIFE_GUT",				WEAPON_KNIFE_GUT },
	{ "WEAPON_KNIFE_KARAMBIT",			WEAPON_KNIFE_KARAMBIT },
	{ "WEAPON_KNIFE_M9_BAYONET",		WEAPON_KNIFE_M9_BAYONET },
	{ "WEAPON_KNIFE_TACTICAL",			WEAPON_KNIFE_TACTICAL },
	{ "WEAPON_KNIFE_FALCHION",			WEAPON_KNIFE_FALCHION },
	{ "WEAPON_KNIFE_SURVIVAL_BOWIE",	WEAPON_KNIFE_SURVIVAL_BOWIE },
	{ "WEAPON_KNIFE_BUTTERFLY",			WEAPON_KNIFE_BUTTERFLY },
	{ "WEAPON_KNIFE_PUSH",				WEAPON_KNIFE_PUSH },
	{ "WEAPON_KNIFE_PARACORD",			WEAPON_KNIFE_PARACORD },
	{ "WEAPON_KNIFE_SURVIVAL",			WEAPON_KNIFE_SURVIVAL },
	{ "WEAPON_KNIFE_URSUS",				WEAPON_KNIFE_URSUS },
	{ "WEAPON_KNIFE_GYPSY_JACKKNIFE",	WEAPON_KNIFE_GYPSY_JACKKNIFE },
	{ "WEAPON_KNIFE_NOMAD",				WEAPON_KNIFE_NOMAD },
	{ "WEAPON_KNIFE_STILETTO",			WEAPON_KNIFE_STILETTO },
	{ "WEAPON_KNIFE_WIDOWMAKER",		WEAPON_KNIFE_WIDOWMAKER },
	{ "WEAPON_KNIFE_SKELETON",			WEAPON_KNIFE_SKELETON },
	{ "GLOVE_STUDDED_BLOODHOUND",		GLOVE_STUDDED_BLOODHOUND },
	{ "GLOVE_T_SIDE",					GLOVE_T_SIDE },
	{ "GLOVE_CT_SIDE",					GLOVE_CT_SIDE },
	{ "GLOVE_SPORTY",					GLOVE_SPORTY },
	{ "GLOVE_SLICK",					GLOVE_SLICK },
	{ "GLOVE_LEATHER_WRAP",				GLOVE_LEATHER_WRAP },
	{ "GLOVE_MOTORCYCLE",				GLOVE_MOTORCYCLE },
	{ "GLOVE_SPECIALIST",				GLOVE_SPECIALIST },
	{ "GLOVE_HYDRA",					GLOVE_HYDRA }
};

const std::unordered_map<std::string, int> all_custom_models = {
	{ "LOCAL_PLAYER",					LOCAL_PLAYER },
	{ "PLAYER_ALLY",					PLAYER_ALLY },
	{ "PLAYER_ENEMY",					PLAYER_ENEMY },
	{ "ARMS",							ARMS }
};

struct collideable_t {
	vec3_t obb_mins() {
		using original_fn = vec3_t * (__thiscall*)(void*);
		return *((*(original_fn**)this)[1](this));
	};
	vec3_t obb_maxs() {
		using original_fn = vec3_t * (__thiscall*)(void*);
		return *((*(original_fn**)this)[2](this));
	};
};

class entity_t {
public:
	void* animating() {
		return reinterpret_cast<void*>(uintptr_t(this) + 0x4);
	}
	void* networkable() {
		return reinterpret_cast<void*>(uintptr_t(this) + 0x8);
	}
	collideable_t* collideable() {
		using original_fn = collideable_t * (__thiscall*)(void*);
		return (*(original_fn**)this)[3](this);
	}
	matrix_t& coordinate_frame() {
		static auto offset = netvar_manager::get_net_var(fnv::hash("DT_BaseEntity"), fnv::hash("m_CollisionGroup")) - 0x30;
		return *reinterpret_cast<matrix_t*>(reinterpret_cast<uint8_t*>(this) + offset);
	}
	c_client_class* client_class() {
		using original_fn = c_client_class * (__thiscall*)(void*);
		return (*(original_fn**)networkable())[2](networkable());
	}
	int index() {
		using original_fn = int(__thiscall*)(void*);
		return (*(original_fn**)networkable())[10](networkable());
	}
	bool is_player() {
		using original_fn = bool(__thiscall*)(entity_t*);
		return (*(original_fn**)this)[158](this);
	}
	bool is_weapon() {
		using original_fn = bool(__thiscall*)(entity_t*);
		return (*(original_fn**)this)[165](this);
	}
	bool setup_bones(matrix_t* out, int max_bones, int mask, float time) {
		if (!this)
			return false;

		using original_fn = bool(__thiscall*)(void*, matrix_t*, int, int, float);
		return (*(original_fn**)animating())[13](animating(), out, max_bones, mask, time);
	}
	model_t* model() {
		using original_fn = model_t * (__thiscall*)(void*);
		return (*(original_fn**)animating())[8](animating());
	}
	void update() {
		using original_fn = void(__thiscall*)(entity_t*);
		(*(original_fn**)this)[218](this);
	}
	int draw_model(int flags, uint8_t alpha) {
		using original_fn = int(__thiscall*)(void*, int, uint8_t);
		return (*(original_fn**)animating())[9](animating(), flags, alpha);
	}
	void set_angles(vec3_t angles) {
		using original_fn = void(__thiscall*)(void*, const vec3_t&);
		static original_fn set_angles_fn = (original_fn)((DWORD)utilities::pattern_scan("client.dll", "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1"));
		set_angles_fn(this, angles);
	}
	void set_position(vec3_t position) {
		using original_fn = void(__thiscall*)(void*, const vec3_t&);
		static original_fn set_position_fn = (original_fn)((DWORD)utilities::pattern_scan("client.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8"));
		set_position_fn(this, position);
	}

	void set_model_index(int index) {
		using original_fn = void(__thiscall*)(void*, int);
		return (*(original_fn**)this)[75](this, index);
	}

	void net_pre_data_update(int update_type) {
		using original_fn = void(__thiscall*)(void*, int);
		return (*(original_fn**)networkable())[6](networkable(), update_type);
	}

	void net_release() {
		using original_fn = void(__thiscall*)(void*);
		return (*(original_fn**)networkable())[1](networkable());
	}

	int net_set_destroyed_on_recreate_entities() {
		using original_fn = int(__thiscall*)(void*);
		return (*(original_fn**)networkable())[13](networkable());
	}

	bool dormant() {
		using original_fn = bool(__thiscall*)(void*);
		return (*static_cast<original_fn**>(networkable()))[9](networkable());
	}

	NETVAR("DT_CSPlayer", "m_fFlags", flags, int);
	NETVAR("DT_BaseEntity", "m_hOwnerEntity", owner_handle, unsigned long);
	NETVAR("DT_CSPlayer", "m_flSimulationTime", simulation_time, float);
	NETVAR("DT_BasePlayer", "m_vecOrigin", origin, vec3_t);
	NETVAR("DT_BasePlayer", "m_vecViewOffset[0]", view_offset, vec3_t);
	NETVAR("DT_CSPlayer", "m_iTeamNum", team, int);
	NETVAR("DT_BaseEntity", "m_bSpotted", spotted, bool);
	NETVAR("DT_CSPlayer", "m_nSurvivalTeam", survival_team, int);
	NETVAR("DT_CSPlayer", "m_flHealthShotBoostExpirationTime", health_boost_time, float);
	NETVAR("DT_PlantedC4", "m_flC4Blow", m_flC4Blow, float);
	NETVAR("DT_PlantedC4", "m_flDefuseCountDown", m_flDefuseCountDown, float);
	NETVAR("DT_PlantedC4", "m_bBombDefused", m_bBombDefused, bool);
	NETVAR("DT_PlantedC4", "m_hBombDefuser", m_hBombDefuser, unsigned long);	// Handle
	NETVAR("DT_PlantedC4", "m_flTimerLength", m_flTimerLength, float);
};

class econ_view_item_t {
public:
	NETVAR("DT_ScriptCreatedItem", "m_bInitialized", is_initialized, bool)
	NETVAR("DT_ScriptCreatedItem", "m_iEntityLevel", entity_level, int)
	NETVAR("DT_ScriptCreatedItem", "m_iAccountID", account_id, int)
	NETVAR("DT_ScriptCreatedItem", "m_iItemIDLow", item_id_low, int)
};

class base_view_model_t : public entity_t {
public:
	NETVAR("DT_BaseViewModel", "m_nModelIndex", model_index, int)
	NETVAR("DT_BaseViewModel", "m_nViewModelIndex", view_model_index, int)
	NETVAR("DT_BaseViewModel", "m_hWeapon", weapon, int)
	NETVAR("DT_BaseViewModel", "m_hOwner", owner, int)
};

class weapon_t : public entity_t {
public:
	#pragma region BaseCombatWeapon
	NETVAR("DT_BaseCombatWeapon", "m_flNextPrimaryAttack", next_primary_attack, float)
	NETVAR("DT_BaseCombatWeapon", "m_flNextSecondaryAttack", next_secondary_attack, float)
	NETVAR("DT_BaseCombatWeapon", "m_iClip1", clip1_count, int)
	NETVAR("DT_BaseCombatWeapon", "m_iClip2", clip2_count, int)
	NETVAR("DT_BaseCombatWeapon", "m_iPrimaryReserveAmmoCount", primary_reserve_ammo_acount, int)
	NETVAR("DT_BaseCombatWeapon", "m_iViewModelIndex", viewmodel_index, int)
	NETVAR("DT_BaseCombatWeapon", "m_iWorldModelIndex", worldmodel_index, int)
	NETVAR("DT_BaseCombatWeapon", "m_iWorldDroppedModelIndex", droppedmodel_index, int)
	NETVAR("DT_BaseCombatWeapon", "m_hWeaponWorldModel", weapon_worldmodel, int)
	#pragma endregion

	NETVAR("DT_WeaponCSBase", "m_flRecoilIndex", recoil_index, float)
	NETVAR("DT_WeaponCSBase", "m_fLastShotTime", last_shot_time, float)
	NETVAR("DT_BaseCSGrenade", "m_bPinPulled", pin_pulled, bool)
	NETVAR("DT_BaseCSGrenade", "m_fThrowTime", throw_time, float)
	NETVAR("DT_WeaponCSBaseGun", "m_zoomLevel", zoom_level, float)

	#pragma region BaseAttributableItem
	NETVAR("DT_BaseAttributableItem", "m_iItemDefinitionIndex", item_definition_index, short)
	NETVAR("DT_BaseAttributableItem", "m_iItemIDHigh", item_id_high, int)
	NETVAR("DT_BaseAttributableItem", "m_iAccountID", account_id, int)
	NETVAR("DT_BaseAttributableItem", "m_iEntityQuality", entity_quality, int)
	NETVAR_PTR("DT_BaseAttributableItem", "m_szCustomName", custom_name, char)
	NETVAR("DT_BaseAttributableItem", "m_nFallbackPaintKit", fallback_paint_kit, int)
	NETVAR("DT_BaseAttributableItem", "m_nFallbackSeed", fallback_seed, int)
	NETVAR("DT_BaseAttributableItem", "m_flFallbackWear", fallback_wear, float)
	NETVAR("DT_BaseAttributableItem", "m_nFallbackStatTrak", fallback_stattrack, int)
	#pragma endregion

	float inaccuracy() {
		using original_fn = float(__thiscall*)(void*);
		return (*(original_fn**)this)[482](this);
	}

	float get_spread() {
		using original_fn = float(__thiscall*)(void*);
		return (*(original_fn**)this)[452](this);
	}

	void update_accuracy_penalty() {
		using original_fn = void(__thiscall*)(void*);
		(*(original_fn**)this)[483](this);
	}

	weapon_info_t* get_weapon_data() {
		return interfaces::weapon_system->get_weapon_data(this->item_definition_index());
	}


	bool is_non_aim() {
		return this->is_bomb() || this->is_knife() || this->is_grenade() || this->clip1_count() == 0;
	}

	bool is_bomb() {
		return this->item_definition_index() == WEAPON_C4;
	}

	bool is_taser() {
		return this->item_definition_index() == WEAPON_TASER;
	}

	bool is_knife() {
		switch (this->item_definition_index()) {
			case WEAPON_KNIFE:
			case WEAPON_KNIFE_T:
			case WEAPON_KNIFEGG:
			case WEAPON_BAYONET:
			case WEAPON_KNIFE_BUTTERFLY:
			case WEAPON_KNIFE_FALCHION:
			case WEAPON_KNIFE_FLIP:
			case WEAPON_KNIFE_GUT:
			case WEAPON_KNIFE_KARAMBIT:
			case WEAPON_KNIFE_M9_BAYONET:
			case WEAPON_KNIFE_PUSH:
			case WEAPON_KNIFE_TACTICAL:
			case WEAPON_KNIFE_SURVIVAL_BOWIE:
			case WEAPON_KNIFE_GYPSY_JACKKNIFE:
			case WEAPON_KNIFE_STILETTO:
			case WEAPON_KNIFE_WIDOWMAKER:
			case WEAPON_KNIFE_SKELETON:
			case WEAPON_KNIFE_URSUS:
			case WEAPON_KNIFE_CSS:
				return true;
			default:
				return false;
		}
	}

	bool is_grenade() {
		switch (this->item_definition_index()) {
			case WEAPON_FLASHBANG:
			case WEAPON_HEGRENADE:
			case WEAPON_SMOKEGRENADE:
			case WEAPON_MOLOTOV:
			case WEAPON_INCGRENADE:
			case WEAPON_DECOY:
				return true;
			default:
				return false;
		}
	}

};

class player_t : public entity_t {
private:
	template <typename T>
	T& read(uintptr_t offset) {
		return *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + offset);
	}

	template <typename T>
	void write(uintptr_t offset, T data) {
		*reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + offset) = data;
	}

public:
	NETVAR("DT_BasePlayer", "m_hViewModel[0]", view_model, int)
	NETVAR("DT_CSPlayer", "m_bHasDefuser", has_defuser, bool)
	NETVAR("DT_CSPlayer", "m_bGunGameImmunity", has_gun_game_immunity, bool)
	NETVAR("DT_CSPlayer", "m_iShotsFired", shots_fired, int)
	NETVAR("DT_CSPlayer", "m_angEyeAngles", eye_angles, vec3_t)
	NETVAR("DT_CSPlayer", "m_ArmorValue", armor, int)
	NETVAR("DT_CSPlayer", "m_bHasHelmet", has_helmet, bool)
	NETVAR("DT_CSPlayer", "m_bIsScoped", is_scoped, bool)
	NETVAR("DT_CSPlayer", "m_bIsDefusing", is_defusing, bool)
	NETVAR("DT_CSPlayer", "m_iAccount", money, int)
	NETVAR("DT_CSPlayer", "m_flLowerBodyYawTarget", lower_body_yaw, float)
	NETVAR("DT_CSPlayer", "m_flNextAttack", next_attack, float)
	NETVAR("DT_CSPlayer", "m_flFlashDuration", flash_duration, float)
	NETVAR("DT_CSPlayer", "m_flFlashMaxAlpha", flash_alpha, float)
	NETVAR("DT_CSPlayer", "m_bHasNightVision", has_night_vision, bool)
	NETVAR("DT_CSPlayer", "m_bNightVisionOn", night_vision_on, bool)
	NETVAR("DT_CSPlayer", "m_iHealth", health, int)
	NETVAR("DT_CSPlayer", "m_lifeState", life_state, int)
	NETVAR("DT_CSPlayer", "m_fFlags", flags, int)
	NETVAR_PTR("DT_CSPlayer", "m_szArmsModel", arms_model, char)
	NETVAR("DT_BasePlayer", "m_viewPunchAngle", punch_angle, vec3_t)
	NETVAR("DT_BasePlayer", "m_aimPunchAngle", aim_punch_angle, vec3_t)
	NETVAR("DT_BasePlayer", "m_vecVelocity[0]", velocity, vec3_t)
	NETVAR("DT_BasePlayer", "m_flMaxspeed", max_speed, float)
	NETVAR("DT_BaseEntity", "m_flShadowCastDistance", fov_time, float)
	NETVAR("DT_BasePlayer", "m_hObserverTarget", observer_target, unsigned long)
	NETVAR("DT_BasePlayer", "m_iObserverMode", observer_mode, int)
	NETVAR("DT_BasePlayer", "m_nHitboxSet", hitbox_set, int)
	NETVAR("DT_BasePlayer", "m_nNextThinkTick", next_think_tick, int)
	NETVAR("DT_BasePlayer", "m_flFallVelocity", fall_velocity, float)
	NETVAR("DT_BaseAnimating", "m_nSequence", get_sequence, int)
	NETVAR("DT_CSPlayer", "m_flDuckAmount", duck_amount, float)
	NETVAR("DT_CSPlayer", "m_bHasHeavyArmor", has_heavy_armor, bool)
	NETVAR("DT_SmokeGrenadeProjectile", "m_nSmokeEffectTickBegin", smoke_grenade_tick_begin, int)
	NETVAR("DT_CSPlayer", "m_nTickBase", get_tick_base, int)

	datamap_t* pred_datamap() {
		using original_fn = datamap_t * (__thiscall*)(void*);
		return (*(original_fn**)this)[17](this);
	}

	FINDDATAMAP(pred_datamap(), "m_hGroundEntity", ground_entity, int)
	FINDDATAMAP(pred_datamap(), "m_MoveType", move_type, int)
	FINDDATAMAP(pred_datamap(), "m_nButtons", buttons, int)
	FINDDATAMAP(pred_datamap(), "m_afButtonLast", button_last, int)
	FINDDATAMAP(pred_datamap(), "m_afButtonPressed", button_pressed, int)
	FINDDATAMAP(pred_datamap(), "m_afButtonReleased", button_released, int)
	FINDDATAMAP(pred_datamap(), "m_nImpulse", impulse, byte)

	c_usercmd** current_command() {
		auto offset = netvar_manager::get_net_var(fnv::hash("CBasePlayer"), fnv::hash("m_hViewEntity")) - 0x4;
		return reinterpret_cast<c_usercmd**>(uintptr_t(this) + offset);
	}

	c_usercmd& last_command() {
		static const std::uintptr_t last_command_fn =
			*reinterpret_cast<std::uintptr_t*>(utilities::pattern_scan("client.dll", "8D 8E ? ? ? ? 89 5C 24 3C") + 2);
		return *reinterpret_cast<c_usercmd*>(reinterpret_cast<std::uintptr_t>(this) + last_command_fn);
	}

	void set_next_think(int think) {
		using original_fn = void(__thiscall*)(void*, int);
		static auto set_next_think_fn = reinterpret_cast<original_fn>(utilities::pattern_scan("client.dll", "55 8B EC 56 57 8B F9 8B B7 ? ? ? ? 8B"));
		set_next_think_fn(this, think);
	}

	bool physics_run_think(int think) {
		using original_fn = bool(__thiscall*)(void*, int);
		static auto physics_run_think_fn = reinterpret_cast<original_fn>(utilities::pattern_scan("client.dll", "55 8B EC 83 EC 10 53 56 57 8B F9 8B 87"));
		return physics_run_think_fn(this, think);
	}

	void post_think_v_physics() {
		using original_fn = void(__thiscall*)(void*);
		static auto post_think_v_physics_fn = reinterpret_cast<original_fn>(utilities::pattern_scan("client.dll", "55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 8B D9 56 57 83 BB"));
		post_think_v_physics_fn(this);
	}

	void simulate_player_simulated_entities() {
		using original_fn = void(__thiscall*)(void*);
		static auto simulate_player_simulated_entities_fn = reinterpret_cast<original_fn>(utilities::pattern_scan("client.dll", "56 8B F1 57 8B BE ? ? ? ? 83 EF 01 78 74"));
		simulate_player_simulated_entities_fn(this);
	}

	weapon_t* active_weapon() {
		auto active_weapon = read<uintptr_t>(netvar_manager::get_net_var(fnv::hash("DT_CSPlayer"), fnv::hash("m_hActiveWeapon"))) & 0xFFF;
		return reinterpret_cast<weapon_t*>(interfaces::entity_list->get_client_entity(active_weapon));
	}

	UINT* get_wearables() {
		return (UINT*)((uintptr_t)this + (netvar_manager::get_net_var(fnv::hash("DT_CSPlayer"), fnv::hash("m_hMyWearables"))));
	}

	UINT* get_weapons() {
		return (UINT*)((uintptr_t)this + (netvar_manager::get_net_var(fnv::hash("DT_CSPlayer"), fnv::hash("m_hMyWeapons"))));
	}

	vec3_t get_eye_pos() {
		return origin() + view_offset();
	}

	anim_state* get_anim_state() {
		return *reinterpret_cast<anim_state**>(this + 0x3914);
	}

	base_view_model_t* get_view_model() {
		return (base_view_model_t*)interfaces::entity_list->get_client_entity_handle(view_model());
	}

	std::uint64_t get_steam_id() {
		if (player_info_t player_info; interfaces::engine->get_player_info(index(), &player_info))
			return player_info.xuid;
		return 0;
	}

	bool can_see_player_pos(player_t* player, const vec3_t& pos) {
		trace_filter filter;					// trace_filter derived from i_trace_filter
		filter.skip = this;						// Add the origin player_t to skip filter

		auto start = get_eye_pos();				// Get eye pos from origin player_t
		auto dir = (pos - start).normalized();	// idk

		ray_t ray;
		ray.initialize(start, pos);				// Initialize ray like this

		trace_t tr;
		interfaces::trace_ray->trace_ray(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr);

		return tr.entity == player || tr.flFraction > 0.97f;	// We found the entity and the fraction is long enough to not collide with shit
	}

	vec3_t get_bone_position(int bone) {
		matrix_t bone_matrices[128];
		if (setup_bones(bone_matrices, 128, 256, 0.0f))
			return vec3_t{ bone_matrices[bone][0][3], bone_matrices[bone][1][3], bone_matrices[bone][2][3] };
		else
			return vec3_t{ };
	}

	vec3_t get_hitbox_position(int hitbox_id) {
		matrix_t bone_matrix[MAXSTUDIOBONES];

		if (setup_bones(bone_matrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0.0f)) {
			auto studio_model = interfaces::model_info->get_studio_model(model());

			if (studio_model) {
				auto hitbox = studio_model->hitbox_set(0)->hitbox(hitbox_id);

				if (hitbox) {
					auto min = vec3_t{}, max = vec3_t{};

					math::transform_vector(hitbox->mins, bone_matrix[hitbox->bone], min);
					math::transform_vector(hitbox->maxs, bone_matrix[hitbox->bone], max);

					return vec3_t((min.x + max.x) * 0.5f, (min.y + max.y) * 0.5f, (min.z + max.z) * 0.5f);
				}
			}
		}
		return vec3_t{};
	}

	// Checks if target is enemy from `this`
	bool is_enemy(player_t* target) {
		if (!target) return false;
		using fn = bool(__thiscall*)(player_t*, player_t*);
		static fn isOtherEnemy = (fn)(utilities::pattern_scan("client.dll", "8B CE E8 ? ? ? ? 02 C0") + 3);
		isOtherEnemy(this, target);
	}

	bool is_alive() {
		if (!this) return false;
		return this->health() > 0;
	}

	bool is_moving() {
		if (!this) return false;
		return this->velocity().length() > 0.1f;
	}

	bool is_in_air() {
		if (!this) return false;
		return !(this->flags() & fl_onground);
	}

	bool is_flashed() {
		if (!this) return false;
		return this->flash_duration() > 0.0f;
	}

	void update_client_side_animations() {
		using original_fn = void(__thiscall*)(void*);
		(*(original_fn**)this)[224](this);
	}

	vec3_t& abs_origin() {
		using original_fn = vec3_t & (__thiscall*)(void*);
		return (*(original_fn**)this)[10](this);;
	}

	vec3_t& abs_angles() {
		using original_fn = vec3_t & (__thiscall*)(void*);
		return (*(original_fn**)this)[11](this);;
	}

	void select_item(const char* str, int sub_type) {
		using original_fn = void(__thiscall*)(void*, const char*, int);
		(*(original_fn**)this)[330](this, str, sub_type);
	}

	void pre_think() {
		using original_fn = void(__thiscall*)(void*);
		(*(original_fn**)this)[318](this);
	}

	void think() {
		using original_fn = void(__thiscall*)(void*);
		(*(original_fn**)this)[139](this);
	}

	void set_sequence(int sequence) {
		using original_fn = void(__thiscall*)(void*, int);
		(*(original_fn**)this)[219](this, sequence);
	}

	void studio_frame_advance() {
		using original_fn = void(__thiscall*)(void*);
		(*(original_fn**)this)[220](this);
	}

	void update_collision_bounds() {
		using original_fn = void(__thiscall*)(void*);
		(*(original_fn**)this)[340](this);
	}
};
