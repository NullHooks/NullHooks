#include "dependencies/utilities/csgo.hpp"
#include "core/config/config.hpp"
#include "core/menu/variables.hpp"
#include "core/features/visuals/skin_changer/skin_changer.hpp"
// Rapidjson
#include "dependencies/rapidjson/document.h"
#include "dependencies/rapidjson/writer.h"
#include "dependencies/rapidjson/stringbuffer.h"

#pragma region LOAD_CONFIG
void config::load_config(std::string filename) {
	std::string full_path = nullhooks_config_folder + "\\config\\" + filename;

	DWORD exitst = GetFileAttributesA(full_path.c_str());
	if (exitst == INVALID_FILE_ATTRIBUTES)		// Path does not exist
		std::ofstream{ full_path.c_str() };		// Open as output to create it

	std::ifstream file;
	file.open(full_path, std::ios::in);
	std::string file_contents;
	if (file.is_open()) {
		std::string line_buffer;
		while (std::getline(file, line_buffer)) {
			file_contents += line_buffer + "\n";
		}
		file.close();
	}

	// File contents as str to dom
	rapidjson::Document doc;
	if (doc.Parse(file_contents.c_str()).HasParseError()) return;

	/* ------------------------ Read all variables ------------------------ */
	// Aim
	load::parse_bool(doc,			variables::aim::triggerbot,								"aim_triggerbot");
	load::parse_hotkey(doc,			variables::aim::triggerbot_key,							"aim_triggerbot_key");
	load::parse_float(doc,			variables::aim::triggerbot_delay,						"aim_triggerbot_delay");
	load::parse_bool(doc,			variables::aim::aimbot,									"aim_aimbot");
	load::parse_bool(doc,			variables::aim::silent,									"aim_silent");
	load::parse_bool(doc,			variables::aim::autofire,								"aim_autofire");
	load::parse_hotkey(doc,			variables::aim::aimbot_key,								"aim_aimbot_key");
	load::parse_combobox(doc,		variables::aim::autowall,								"aim_autowall");
	load::parse_float(doc,			variables::aim::aimbot_fov,								"aim_aimbot_fov");
	load::parse_float(doc,			variables::aim::aimbot_smoothing,						"aim_aimbot_smoothing");
	load::parse_bool(doc,			variables::aim::aimbot_isvisiblecheck,					"aim_aimbot_isvisiblecheck");
	load::parse_bool(doc,			variables::aim::target_friends,							"aim_target_friends");
	load::parse_bool(doc,			variables::aim::non_rifle_aimpunch,						"aim_non_rifle_aimpunch");
	load::parse_bool(doc,			variables::aim::aimbot_noscope,							"aim_aimbot_noscope");
	load::parse_float(doc,			variables::aim::min_damage,								"aim_min_damage");
	load::parse_multicombo(doc,		variables::aim::hitboxes,								"aim_hitboxes");
	// Player visuals
	load::parse_bool(doc,			variables::player_visuals::showteamesp,					"visuals_showteamesp");
	load::parse_bool(doc,			variables::player_visuals::playerglow,					"visuals_playerglow");
	load::parse_bool(doc,			variables::player_visuals::boxesp,						"visuals_boxesp");
	load::parse_bool(doc,			variables::player_visuals::lineesp,						"visuals_lineesp");
	load::parse_bool(doc,			variables::player_visuals::skeletonesp,					"visuals_skeletonesp");
	load::parse_bool(doc,			variables::player_visuals::nameesp,						"visuals_nameesp");
	load::parse_bool(doc,			variables::player_visuals::playerinfo,					"visuals_playerinfo");
	load::parse_bool(doc,			variables::player_visuals::healthesp,					"visuals_healthesp");
	// Entity visuals
	load::parse_bool(doc,			variables::entity_visuals::entityglow,					"visuals_entityglow");
	load::parse_bool(doc,			variables::entity_visuals::entitytext,					"visuals_entitytext");
	load::parse_bool(doc,			variables::entity_visuals::bombtimer,					"visuals_bombtimer");
	load::parse_bool(doc,			variables::entity_visuals::nade_esp,					"visuals_nade_esp");
	// Chams
	load::parse_bool(doc,			variables::chams::only_visible_chams,					"chams_only_visible_chams");
	load::parse_bool(doc,			variables::chams::draw_chams_on_top,					"chams_draw_chams_on_top");
	load::parse_bool(doc,			variables::chams::wireframe_chams,						"chams_wireframe_chams");
	load::parse_bool(doc,			variables::chams::player_chams,							"chams_player_chams");
	load::parse_bool(doc,			variables::chams::localplayer_chams,					"chams_localplayer_chams");
	load::parse_bool(doc,			variables::chams::vm_weapon_chams,						"chams_vm_weapon_chams");
	load::parse_bool(doc,			variables::chams::vm_arm_chams,							"chams_vm_arm_chams");
	load::parse_bool(doc,			variables::chams::vm_sleeve_chams,						"chams_vm_sleeve_chams");
	load::parse_combobox(doc,		variables::chams::player_chams_mat_id,					"chams_player_mat_id");
	load::parse_combobox(doc,		variables::chams::localplayer_chams_mat_id,				"chams_localplayer_mat_id");
	load::parse_combobox(doc,		variables::chams::weapon_chams_mat_id,					"chams_weapon_mat_id");
	load::parse_combobox(doc,		variables::chams::arm_chams_mat_id,						"chams_arms_mat_id");
	load::parse_combobox(doc,		variables::chams::sleeve_chams_mat_id,					"chams_sleeve_mat_id");
	load::parse_bool(doc,			variables::chams::backtrack_chams,						"chams_backtrack_chams");
	// Misc visuals
	load::parse_float(doc,			variables::misc_visuals::noflash_alpha,					"visuals_noflash");
	load::parse_bool(doc,			variables::misc_visuals::wireframe_smoke,				"visuals_wireframe_smoke");
	load::parse_bool(doc,			variables::misc_visuals::nade_predict,					"visuals_nade_predict");
	load::parse_bool(doc,			variables::misc_visuals::noscope,						"visuals_noscope");
	load::parse_bool(doc,			variables::misc_visuals::crosshair,						"visuals_crosshair");
	load::parse_bool(doc,			variables::misc_visuals::recoil_crosshair,				"visuals_recoil_crosshair");
	load::parse_multicombo(doc,		variables::misc_visuals::bulletracer_draw_target,		"visuals_bulletracer_draw_target");
	load::parse_multicombo(doc,		variables::misc_visuals::bulletracer_team_target,		"visuals_bulletracer_team_target");
	load::parse_bool(doc,			variables::misc_visuals::chickenpride,					"visuals_chickenpride");
	load::parse_float(doc,			variables::misc_visuals::custom_fov_slider,				"visuals_custom_fov_slider");
	load::parse_float(doc,			variables::misc_visuals::custom_vmfov_slider,			"visuals_custom_vmfov_slider");
	// Misc
	load::parse_bool(doc,			variables::misc::infinite_duck,							"misc_infinite_duck");
	load::parse_bool(doc,			variables::misc::bhop,									"misc_bhop");
	load::parse_combobox(doc,		variables::misc::autostrafe_target,						"misc_autostrafe");
	load::parse_bool(doc,			variables::misc::edgejump,								"misc_edgejump");
	load::parse_hotkey(doc,			variables::misc::ej_key,								"misc_ej_key");
	load::parse_bool(doc,			variables::misc::edgebug,								"misc_edgebug");
	load::parse_hotkey(doc,			variables::misc::eb_key,								"misc_eb_key");
	load::parse_bool(doc,			variables::misc::jumpbug,								"misc_jumpbug");
	load::parse_hotkey(doc,			variables::misc::jb_key,								"misc_jb_key");
	load::parse_bool(doc,			variables::misc::slowwalk,								"misc_slowwalk");
	load::parse_hotkey(doc,			variables::misc::slowwalk_key,							"misc_slowwalk_key");
	load::parse_multicombo(doc,		variables::misc::speedgraph_target,						"misc_speedgraph_target");
	load::parse_float(doc,			variables::misc::speedgraph_h,							"misc_speedgraph_h");
	load::parse_float(doc,			variables::misc::speedgraph_pos,						"misc_speedgraph_pos");
	load::parse_bool(doc,			variables::misc::backtrack,								"misc_backtrack");
	load::parse_bool(doc,			variables::misc::backtrack_team,						"misc_backtrack_team");
	load::parse_bool(doc,			variables::misc::thirdperson,							"misc_thirdperson");
	load::parse_hotkey(doc,			variables::misc::thirdperson_key,						"misc_thirdperson_key");
	load::parse_float(doc,			variables::misc::thirdperson_dist,						"misc_thirdperson_dist");
	load::parse_bool(doc,			variables::misc::draw_watermark,						"misc_draw_watermark");
	load::parse_bool(doc,			variables::misc::draw_stats,							"misc_draw_stats");
	load::parse_bool(doc,			variables::misc::clean_screenshots,						"misc_clean_screenshots");
	// Ui
	load::parse_bool(doc,			variables::ui::spectators::spectator_list,				"ui_spectator_list");
	// Colors
	load::parse_color(doc,			variables::colors::friendly_color,						"color_friendly_color");
	load::parse_color(doc,			variables::colors::friendly_color_soft,					"color_friendly_color_soft");
	load::parse_color(doc,			variables::colors::friendly_color_softer,				"color_friendly_color_softer");
	load::parse_color(doc,			variables::colors::enemy_color,							"color_enemy_color");
	load::parse_color(doc,			variables::colors::enemy_color_soft,					"color_enemy_color_soft");
	load::parse_color(doc,			variables::colors::enemy_color_softer,					"color_enemy_color_softer");
	load::parse_color(doc,			variables::colors::enemy_color_softer,					"color_enemy_color_softer");
	load::parse_color(doc,			variables::colors::chams_localplayer,					"color_chams_localplayer");
	load::parse_color(doc,			variables::colors::chams_vis_enemy_c,					"color_chams_vis_enemy_c");
	load::parse_color(doc,			variables::colors::chams_inv_enemy_c,					"color_chams_inv_enemy_c");
	load::parse_color(doc,			variables::colors::chams_vis_friend_c,					"color_chams_vis_friend_c");
	load::parse_color(doc,			variables::colors::chams_inv_friend_c,					"color_chams_inv_friend_c");
	load::parse_color(doc,			variables::colors::chams_weapon_c,						"color_chams_weapon_c");
	load::parse_color(doc,			variables::colors::chams_arms_c,						"color_chams_arms_c");
	load::parse_color(doc,			variables::colors::chams_sleeve_c,						"color_chams_sleeve_c");
	load::parse_color(doc,			variables::colors::bt_chams_enemy,						"color_bt_chams_enemy");
	load::parse_color(doc,			variables::colors::bt_chams_friend,						"color_bt_chams_friend");
	load::parse_color(doc,			variables::colors::crosshair_c,							"color_crosshair_c");
	load::parse_color(doc,			variables::colors::recoil_crosshair_c,					"color_recoil_crosshair_c");
	// Motion blur
	load::parse_bool(doc,			variables::motion_blur.enabled,							"motion_blur_enabled");
	load::parse_bool(doc,			variables::motion_blur.forwardEnabled,					"motion_blur_forward_enabled");
	load::parse_float(doc,			variables::motion_blur.fallingMin,						"motion_blur_falling_min");
	load::parse_float(doc,			variables::motion_blur.fallingMax,						"motion_blur_falling_max");
	load::parse_float(doc,			variables::motion_blur.fallingIntensity,				"motion_blur_falling_intensity");
	load::parse_float(doc,			variables::motion_blur.rotationIntensity,				"motion_blur_rotation_intensity");
	load::parse_float(doc,			variables::motion_blur.strength,						"motion_blur_strength");
}

void config::test_config() {
	load_config("config1.json");
}
#pragma endregion

#pragma region LOAD_FUNCTIONS
void config::load::parse_bool(rapidjson::Document& doc, bool& target, std::string json_name) {
	if (!doc.HasMember(json_name.c_str())) return;			// Check if item in doc
	rapidjson::Value& value = doc[json_name.c_str()];		// value will be each json entry of the item

	if (value.IsBool()) target = value.GetBool();
}

void config::load::parse_float(rapidjson::Document& doc, float& target, std::string json_name) {
	if (!doc.HasMember(json_name.c_str())) return;			// Check if item in doc
	rapidjson::Value& value = doc[json_name.c_str()];		// value will be each json entry of the item

	if (value.IsFloat()) target = value.GetFloat();
}

void config::load::parse_combobox(rapidjson::Document& doc, combobox_toggle_t& target, std::string json_name) {
	if (!doc.HasMember(json_name.c_str())) return;			// Check if item in doc
	rapidjson::Value& value = doc[json_name.c_str()];		// value will be each json entry of the item

	if (value.IsInt()) target.idx = value.GetInt();
}

void config::load::parse_multicombo(rapidjson::Document& doc, multicombobox_toggle_t& target, std::string json_name) {
	if (!doc.HasMember(json_name.c_str())) return;			// Check if item in doc
	rapidjson::Value& value = doc[json_name.c_str()];		// value will be each json entry of the item

	if (value.IsArray()) {											// Check item is array
		for (int n = 0; n < target.vector.size(); n++) {			// Each target vector item
			if (n < value.Size()) {									// Json array is good
				if (value[n].IsBool())								// Check json value is a bool
					target.vector[n].state = value[n].GetBool();	// Assign to target
			}
		}
	}
}

void config::load::parse_color(rapidjson::Document& doc, colorpicker_col_t& target, std::string json_name) {
	if (!doc.HasMember(json_name.c_str())) return;			// Check if item in doc
	rapidjson::Value& value = doc[json_name.c_str()];		// value will be each json entry of the item

	if (value.IsArray()) {											// Check item is array
		if (value.Size() > 0 && value[0].IsInt())					// Json array is good and json value is a bool
			target.col.r = value[0].GetInt();						// (R) Assign to color parameter
		if (value.Size() > 1 && value[1].IsInt())
			target.col.g = value[1].GetInt();						// (G)
		if (value.Size() > 2 && value[2].IsInt())
			target.col.b = value[2].GetInt();						// (B)
		if (value.Size() > 3 && value[3].IsInt())
			target.col.a = value[3].GetInt();						// (A)
	}
}

void config::load::parse_hotkey(rapidjson::Document& doc, hotkey_t& target, std::string json_name) {
	if (!doc.HasMember(json_name.c_str())) return;			// Check if item in doc
	rapidjson::Value& value = doc[json_name.c_str()];		// value will be each json entry of the item

	if (value.IsInt()) target.key = value.GetInt();
}
#pragma endregion
