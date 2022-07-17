#include "dependencies/utilities/csgo.hpp"
#include "core/config/config.hpp"
#include "core/menu/variables.hpp"
#include "core/features/visuals/skin_changer/skin_changer.hpp"
// Rapidjson
#include "dependencies/rapidjson/document.h"
#include "dependencies/rapidjson/writer.h"
#include "dependencies/rapidjson/stringbuffer.h"

#pragma region SAVE_CONFIG
void config::save_config(std::string filename) {
	std::string full_path = nullhooks_config_folder + "\\config\\" + filename;

	DWORD exitst = GetFileAttributesA(full_path.c_str());
	if (exitst == INVALID_FILE_ATTRIBUTES)		// Path does not exist
		std::ofstream{ full_path.c_str() };		// Open as output to create it

	// Docuement for savin the values
	rapidjson::Document doc;												// Main json object
	doc.SetObject();
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();		// Needed for saving items
	size_t sz = allocator.Size();

	/* ------------------------ Read all variables ------------------------ */
	// Aim
	save::parse_bool(doc, allocator,			variables::aim::triggerbot,								"aim_triggerbot");
	save::parse_hotkey(doc, allocator,			variables::aim::triggerbot_key,							"aim_triggerbot_key");
	save::parse_float(doc, allocator,			variables::aim::triggerbot_delay,						"aim_triggerbot_delay");
	save::parse_bool(doc, allocator,			variables::aim::aimbot,									"aim_aimbot");
	save::parse_bool(doc, allocator,			variables::aim::silent,									"aim_silent");
	save::parse_bool(doc, allocator,			variables::aim::autofire,								"aim_autofire");
	save::parse_hotkey(doc, allocator,			variables::aim::aimbot_key,								"aim_aimbot_key");
	save::parse_combobox(doc, allocator,		variables::aim::autowall,								"aim_autowall");
	save::parse_float(doc, allocator,			variables::aim::aimbot_fov,								"aim_aimbot_fov");
	save::parse_float(doc, allocator,			variables::aim::aimbot_smoothing,						"aim_aimbot_smoothing");
	save::parse_bool(doc, allocator,			variables::aim::aimbot_isvisiblecheck,					"aim_aimbot_isvisiblecheck");
	save::parse_bool(doc, allocator,			variables::aim::target_friends,							"aim_target_friends");
	save::parse_bool(doc, allocator,			variables::aim::non_rifle_aimpunch,						"aim_non_rifle_aimpunch");
	save::parse_bool(doc, allocator,			variables::aim::aimbot_noscope,							"aim_aimbot_noscope");
	save::parse_float(doc, allocator,			variables::aim::min_damage,								"aim_min_damage");
	save::parse_multicombo(doc, allocator,		variables::aim::hitboxes,								"aim_hitboxes");
	// Player visuals
	save::parse_bool(doc, allocator,			variables::player_visuals::showteamesp,					"visuals_showteamesp");
	save::parse_bool(doc, allocator,			variables::player_visuals::playerglow,					"visuals_playerglow");
	save::parse_bool(doc, allocator,			variables::player_visuals::boxesp,						"visuals_boxesp");
	save::parse_bool(doc, allocator,			variables::player_visuals::lineesp,						"visuals_lineesp");
	save::parse_bool(doc, allocator,			variables::player_visuals::skeletonesp,					"visuals_skeletonesp");
	save::parse_bool(doc, allocator,			variables::player_visuals::nameesp,						"visuals_nameesp");
	save::parse_bool(doc, allocator,			variables::player_visuals::playerinfo,					"visuals_playerinfo");
	save::parse_bool(doc, allocator,			variables::player_visuals::healthesp,					"visuals_healthesp");
	// Entity visuals
	save::parse_bool(doc, allocator,			variables::entity_visuals::entityglow,					"visuals_entityglow");
	save::parse_bool(doc, allocator,			variables::entity_visuals::entitytext,					"visuals_entitytext");
	save::parse_bool(doc, allocator,			variables::entity_visuals::bombtimer,					"visuals_bombtimer");
	save::parse_bool(doc, allocator,			variables::entity_visuals::nade_esp,					"visuals_nade_esp");
	// Chams
	save::parse_bool(doc, allocator,			variables::chams::only_visible_chams,					"chams_only_visible_chams");
	save::parse_bool(doc, allocator,			variables::chams::draw_chams_on_top,					"chams_draw_chams_on_top");
	save::parse_bool(doc, allocator,			variables::chams::wireframe_chams,						"chams_wireframe_chams");
	save::parse_bool(doc, allocator,			variables::chams::player_chams,							"chams_player_chams");
	save::parse_bool(doc, allocator,			variables::chams::localplayer_chams,					"chams_localplayer_chams");
	save::parse_bool(doc, allocator,			variables::chams::vm_weapon_chams,						"chams_vm_weapon_chams");
	save::parse_bool(doc, allocator,			variables::chams::vm_arm_chams,							"chams_vm_arm_chams");
	save::parse_bool(doc, allocator,			variables::chams::vm_sleeve_chams,						"chams_vm_sleeve_chams");
	save::parse_combobox(doc, allocator,		variables::chams::player_chams_mat_id,					"chams_player_mat_id");
	save::parse_combobox(doc, allocator,		variables::chams::localplayer_chams_mat_id,				"chams_localplayer_mat_id");
	save::parse_combobox(doc, allocator,		variables::chams::weapon_chams_mat_id,					"chams_weapon_mat_id");
	save::parse_combobox(doc, allocator,		variables::chams::arm_chams_mat_id,						"chams_arms_mat_id");
	save::parse_combobox(doc, allocator,		variables::chams::sleeve_chams_mat_id,					"chams_sleeve_mat_id");
	save::parse_bool(doc, allocator,			variables::chams::backtrack_chams,						"chams_backtrack_chams");
	// Misc visuals
	save::parse_float(doc, allocator,			variables::misc_visuals::noflash_alpha,					"visuals_noflash");
	save::parse_bool(doc, allocator,			variables::misc_visuals::wireframe_smoke,				"visuals_wireframe_smoke");
	save::parse_bool(doc, allocator,			variables::misc_visuals::nade_predict,					"visuals_nade_predict");
	save::parse_bool(doc, allocator,			variables::misc_visuals::noscope,						"visuals_noscope");
	save::parse_bool(doc, allocator,			variables::misc_visuals::crosshair,						"visuals_crosshair");
	save::parse_bool(doc, allocator,			variables::misc_visuals::recoil_crosshair,				"visuals_recoil_crosshair");
	save::parse_multicombo(doc, allocator,		variables::misc_visuals::bulletracer_draw_target,		"visuals_bulletracer_draw_target");
	save::parse_multicombo(doc, allocator,		variables::misc_visuals::bulletracer_team_target,		"visuals_bulletracer_team_target");
	save::parse_bool(doc, allocator,			variables::misc_visuals::chickenpride,					"visuals_chickenpride");
	save::parse_float(doc, allocator,			variables::misc_visuals::custom_fov_slider,				"visuals_custom_fov_slider");
	save::parse_float(doc, allocator,			variables::misc_visuals::custom_vmfov_slider,			"visuals_custom_vmfov_slider");
	// Misc
	save::parse_bool(doc, allocator,			variables::misc::infinite_duck,							"misc_infinite_duck");
	save::parse_bool(doc, allocator,			variables::misc::bhop,									"misc_bhop");
	save::parse_combobox(doc, allocator,		variables::misc::autostrafe_target,						"misc_autostrafe");
	save::parse_bool(doc, allocator,			variables::misc::edgejump,								"misc_edgejump");
	save::parse_hotkey(doc, allocator,			variables::misc::ej_key,								"misc_ej_key");
	save::parse_bool(doc, allocator,			variables::misc::edgebug,								"misc_edgebug");
	save::parse_hotkey(doc, allocator,			variables::misc::eb_key,								"misc_eb_key");
	save::parse_bool(doc, allocator,			variables::misc::jumpbug,								"misc_jumpbug");
	save::parse_hotkey(doc, allocator,			variables::misc::jb_key,								"misc_jb_key");
	save::parse_bool(doc, allocator,			variables::misc::slowwalk,								"misc_slowwalk");
	save::parse_hotkey(doc, allocator,			variables::misc::slowwalk_key,							"misc_slowwalk_key");
	save::parse_multicombo(doc, allocator,		variables::misc::speedgraph_target,						"misc_speedgraph_target");
	save::parse_float(doc, allocator,			variables::misc::speedgraph_h,							"misc_speedgraph_h");
	save::parse_float(doc, allocator,			variables::misc::speedgraph_pos,						"misc_speedgraph_pos");
	save::parse_bool(doc, allocator,			variables::misc::backtrack,								"misc_backtrack");
	save::parse_bool(doc, allocator,			variables::misc::backtrack_team,						"misc_backtrack_team");
	save::parse_bool(doc, allocator,			variables::misc::thirdperson,							"misc_thirdperson");
	save::parse_hotkey(doc, allocator,			variables::misc::thirdperson_key,						"misc_thirdperson_key");
	save::parse_float(doc, allocator,			variables::misc::thirdperson_dist,						"misc_thirdperson_dist");
	save::parse_bool(doc, allocator,			variables::misc::draw_watermark,						"misc_draw_watermark");
	save::parse_bool(doc, allocator,			variables::misc::draw_stats,							"misc_draw_stats");
	save::parse_bool(doc, allocator,			variables::misc::clean_screenshots,						"misc_clean_screenshots");
	// Ui
	save::parse_bool(doc, allocator,			variables::ui::spectators::spectator_list,				"ui_spectator_list");
	// Colors
	save::parse_color(doc, allocator,			variables::colors::friendly_color,						"color_friendly_color");
	save::parse_color(doc, allocator,			variables::colors::friendly_color_soft,					"color_friendly_color_soft");
	save::parse_color(doc, allocator,			variables::colors::friendly_color_softer,				"color_friendly_color_softer");
	save::parse_color(doc, allocator,			variables::colors::enemy_color,							"color_enemy_color");
	save::parse_color(doc, allocator,			variables::colors::enemy_color_soft,					"color_enemy_color_soft");
	save::parse_color(doc, allocator,			variables::colors::enemy_color_softer,					"color_enemy_color_softer");
	save::parse_color(doc, allocator,			variables::colors::enemy_color_softer,					"color_enemy_color_softer");
	save::parse_color(doc, allocator,			variables::colors::chams_localplayer,					"color_chams_localplayer");
	save::parse_color(doc, allocator,			variables::colors::chams_vis_enemy_c,					"color_chams_vis_enemy_c");
	save::parse_color(doc, allocator,			variables::colors::chams_inv_enemy_c,					"color_chams_inv_enemy_c");
	save::parse_color(doc, allocator,			variables::colors::chams_vis_friend_c,					"color_chams_vis_friend_c");
	save::parse_color(doc, allocator,			variables::colors::chams_inv_friend_c,					"color_chams_inv_friend_c");
	save::parse_color(doc, allocator,			variables::colors::chams_weapon_c,						"color_chams_weapon_c");
	save::parse_color(doc, allocator,			variables::colors::chams_arms_c,						"color_chams_arms_c");
	save::parse_color(doc, allocator,			variables::colors::chams_sleeve_c,						"color_chams_sleeve_c");
	save::parse_color(doc, allocator,			variables::colors::bt_chams_enemy,						"color_bt_chams_enemy");
	save::parse_color(doc, allocator,			variables::colors::bt_chams_friend,						"color_bt_chams_friend");
	save::parse_color(doc, allocator,			variables::colors::crosshair_c,							"color_crosshair_c");
	save::parse_color(doc, allocator,			variables::colors::recoil_crosshair_c,					"color_recoil_crosshair_c");
	// Motion blur
	save::parse_bool(doc, allocator,			variables::motion_blur.enabled,							"motion_blur_enabled");
	save::parse_bool(doc, allocator,			variables::motion_blur.forwardEnabled,					"motion_blur_forward_enabled");
	save::parse_float(doc, allocator,			variables::motion_blur.fallingMin,						"motion_blur_falling_min");
	save::parse_float(doc, allocator,			variables::motion_blur.fallingMax,						"motion_blur_falling_max");
	save::parse_float(doc, allocator,			variables::motion_blur.fallingIntensity,				"motion_blur_falling_intensity");
	save::parse_float(doc, allocator,			variables::motion_blur.rotationIntensity,				"motion_blur_rotation_intensity");
	save::parse_float(doc, allocator,			variables::motion_blur.strength,						"motion_blur_strength");

	/* ------------------------ Write to file ------------------------ */

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);

	std::ofstream file;
	file.open(full_path, std::ios::out);
	if (file.is_open()) {
		file << buffer.GetString();
		file.close();
	}
}

void config::test_config2() {
	save_config("config1.json");
}
#pragma endregion

#pragma region SAVE_FUNCTIONS
void config::save::parse_bool(rapidjson::Document& doc, rapidjson::Document::AllocatorType& allocator, bool& target, std::string json_name) {
	rapidjson::Value name(json_name.c_str(), allocator);
	doc.AddMember(name, target, allocator);
}

void config::save::parse_float(rapidjson::Document& doc, rapidjson::Document::AllocatorType& allocator, float& target, std::string json_name) {
	rapidjson::Value name(json_name.c_str(), allocator);
	doc.AddMember(name, target, allocator);
}

void config::save::parse_combobox(rapidjson::Document& doc, rapidjson::Document::AllocatorType& allocator, combobox_toggle_t& target, std::string json_name) {
	rapidjson::Value name(json_name.c_str(), allocator);
	doc.AddMember(name, target.idx, allocator);
}

void config::save::parse_multicombo(rapidjson::Document& doc, rapidjson::Document::AllocatorType& allocator, multicombobox_toggle_t& target, std::string json_name) {
	rapidjson::Value name(json_name.c_str(), allocator);
	rapidjson::Value arr(rapidjson::kArrayType);

	for (int n = 0; n < target.vector.size(); n++) {					// Each target vector item
		arr.PushBack(target.vector.at(n).state, allocator);		// Add item to array
	}

	// Add array to main doc
	doc.AddMember(name, arr, allocator);
}

void config::save::parse_color(rapidjson::Document& doc, rapidjson::Document::AllocatorType& allocator, colorpicker_col_t& target, std::string json_name) {
	rapidjson::Value name(json_name.c_str(), allocator);
	rapidjson::Value arr(rapidjson::kArrayType);

	// Add colors to array in order
	arr.PushBack(target.col.r, allocator);
	arr.PushBack(target.col.g, allocator);
	arr.PushBack(target.col.b, allocator);
	arr.PushBack(target.col.a, allocator);

	// Add array to main doc
	doc.AddMember(name, arr, allocator);
}

void config::save::parse_hotkey(rapidjson::Document& doc, rapidjson::Document::AllocatorType& allocator, hotkey_t& target, std::string json_name) {
	rapidjson::Value name(json_name.c_str(), allocator);
	doc.AddMember(name, target.key, allocator);
}
#pragma endregion
