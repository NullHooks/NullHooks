#include "dependencies/utilities/csgo.hpp"
#include "core/config/config.hpp"
#include "core/menu/variables.hpp"
#include "core/features/visuals/skin_changer/skin_changer.hpp"
// Rapidjson
#include "dependencies/rapidjson/document.h"
#include "dependencies/rapidjson/writer.h"
#include "dependencies/rapidjson/stringbuffer.h"

void config::load_selected_config() {
	if (selected_config >= 0 && selected_config < config_names.size())
		load_config(config_names.at(selected_config));
}

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
	load::parse_bool(doc,			variables::aim::triggerbot,								"aim",				"triggerbot");
	load::parse_hotkey(doc,			variables::aim::triggerbot_key,							"aim",				"triggerbot_key");
	load::parse_float(doc,			variables::aim::triggerbot_delay,						"aim",				"triggerbot_delay");
	load::parse_bool(doc,			variables::aim::aimbot,									"aim",				"aimbot");
	load::parse_bool(doc,			variables::aim::silent,									"aim",				"silent");
	load::parse_bool(doc,			variables::aim::autofire,								"aim",				"autofire");
	load::parse_hotkey(doc,			variables::aim::aimbot_key,								"aim",				"aimbot_key");
	load::parse_bool(doc,			variables::aim::bodyaim_if_lethal,						"aim",				"bodyaim_if_lethal");
	load::parse_combobox(doc,		variables::aim::autowall,								"aim",				"autowall");
	load::parse_float(doc,			variables::aim::aimbot_fov,								"aim",				"aimbot_fov");
	load::parse_bool(doc,			variables::aim::draw_fov,								"aim",				"draw_fov");
	load::parse_float(doc,			variables::aim::aimbot_smoothing,						"aim",				"aimbot_smoothing");
	load::parse_bool(doc,			variables::aim::target_friends,							"aim",				"target_friends");
	load::parse_bool(doc,			variables::aim::non_rifle_aimpunch,						"aim",				"non_rifle_aimpunch");
	load::parse_bool(doc,			variables::aim::aimbot_noscope,							"aim",				"aimbot_noscope");
	load::parse_float(doc,			variables::aim::min_damage,								"aim",				"min_damage");
	load::parse_multicombo(doc,		variables::aim::hitboxes,								"aim",				"hitboxes");
	// Antiaim
	load::parse_bool(doc,			variables::antiaim::antiaim,							"antiaim",			"antiaim");
	load::parse_float(doc,			variables::antiaim::yaw,								"antiaim",			"antiaim_yaw");
	load::parse_float(doc,			variables::antiaim::pitch,								"antiaim",			"antiaim_pitch");
	load::parse_bool(doc,			variables::antiaim::spinbot,							"antiaim",			"spinbot");
	load::parse_float(doc,			variables::antiaim::spinbot_speed,						"antiaim",			"spinbot_speed");
	load::parse_bool(doc,			variables::antiaim::peek_aa,							"antiaim",			"peek_aa");
	load::parse_hotkey(doc,			variables::antiaim::peek_aa_toggle_key,					"antiaim",			"peek_aa_toggle_key");
	// Player visuals
	load::parse_bool(doc,			variables::player_visuals::showteamesp,					"player_visuals",	"showteamesp");
	load::parse_bool(doc,			variables::player_visuals::playerglow,					"player_visuals",	"playerglow");
	load::parse_bool(doc,			variables::player_visuals::boxesp,						"player_visuals",	"boxesp");
	load::parse_bool(doc,			variables::player_visuals::lineesp,						"player_visuals",	"lineesp");
	load::parse_bool(doc,			variables::player_visuals::skeletonesp,					"player_visuals",	"skeletonesp");
	load::parse_bool(doc,			variables::player_visuals::nameesp,						"player_visuals",	"nameesp");
	load::parse_multicombo(doc,		variables::player_visuals::playerinfo,					"player_visuals",	"playerinfo");
	load::parse_bool(doc,			variables::player_visuals::healthesp,					"player_visuals",	"healthesp");
	// Entity visuals
	load::parse_bool(doc,			variables::entity_visuals::entityglow,					"entity_visuals",	"entityglow");
	load::parse_bool(doc,			variables::entity_visuals::entitytext,					"entity_visuals",	"entitytext");
	load::parse_bool(doc,			variables::entity_visuals::bombtimer,					"entity_visuals",	"bombtimer");
	load::parse_bool(doc,			variables::entity_visuals::nade_esp,					"entity_visuals",	"nade_esp");
	// Chams
	load::parse_bool(doc,			variables::chams::only_visible_chams,					"chams",			"only_visible_chams");
	load::parse_bool(doc,			variables::chams::draw_chams_on_top,					"chams",			"draw_chams_on_top");
	load::parse_bool(doc,			variables::chams::wireframe_chams,						"chams",			"wireframe_chams");
	load::parse_bool(doc,			variables::chams::player_chams,							"chams",			"player_chams");
	load::parse_bool(doc,			variables::chams::localplayer_chams,					"chams",			"localplayer_chams");
	load::parse_bool(doc,			variables::chams::vm_weapon_chams,						"chams",			"vm_weapon_chams");
	load::parse_bool(doc,			variables::chams::vm_arm_chams,							"chams",			"vm_arm_chams");
	load::parse_bool(doc,			variables::chams::vm_sleeve_chams,						"chams",			"vm_sleeve_chams");
	load::parse_combobox(doc,		variables::chams::player_chams_mat_id,					"chams",			"player_mat_id");
	load::parse_combobox(doc,		variables::chams::localplayer_chams_mat_id,				"chams",			"localplayer_mat_id");
	load::parse_combobox(doc,		variables::chams::weapon_chams_mat_id,					"chams",			"weapon_mat_id");
	load::parse_combobox(doc,		variables::chams::arm_chams_mat_id,						"chams",			"arms_mat_id");
	load::parse_combobox(doc,		variables::chams::sleeve_chams_mat_id,					"chams",			"sleeve_mat_id");
	load::parse_bool(doc,			variables::chams::backtrack_chams,						"chams",			"backtrack_chams");
	// Misc visuals
	load::parse_float(doc,			variables::misc_visuals::noflash_alpha,					"misc_visuals",		"noflash");
	load::parse_bool(doc,			variables::misc_visuals::wireframe_smoke,				"misc_visuals",		"wireframe_smoke");
	load::parse_bool(doc,			variables::misc_visuals::nade_predict,					"misc_visuals",		"nade_predict");
	load::parse_bool(doc,			variables::misc_visuals::noscope,						"misc_visuals",		"noscope");
	load::parse_bool(doc,			variables::misc_visuals::crosshair,						"misc_visuals",		"crosshair");
	load::parse_bool(doc,			variables::misc_visuals::recoil_crosshair,				"misc_visuals",		"recoil_crosshair");
	load::parse_multicombo(doc,		variables::misc_visuals::bulletracer_draw_target,		"misc_visuals",		"bulletracer_draw_target");
	load::parse_multicombo(doc,		variables::misc_visuals::bulletracer_team_target,		"misc_visuals",		"bulletracer_team_target");
	load::parse_bool(doc,			variables::misc_visuals::chickenpride,					"misc_visuals",		"chickenpride");
	load::parse_float(doc,			variables::misc_visuals::custom_fov_slider,				"misc_visuals",		"custom_fov_slider");
	load::parse_float(doc,			variables::misc_visuals::custom_vmfov_slider,			"misc_visuals",		"custom_vmfov_slider");
	load::parse_bool(doc,			variables::misc_visuals::worldcolor,					"misc_visuals",		"worldcolor");
	// Misc
	load::parse_bool(doc,			variables::misc::infinite_duck,							"misc",				"infinite_duck");
	load::parse_bool(doc,			variables::misc::bhop,									"misc",				"bhop");
	load::parse_combobox(doc,		variables::misc::autostrafe_target,						"misc",				"autostrafe");
	load::parse_bool(doc,			variables::misc::edgejump,								"misc",				"edgejump");
	load::parse_hotkey(doc,			variables::misc::ej_key,								"misc",				"ej_key");
	load::parse_bool(doc,			variables::misc::edgebug,								"misc",				"edgebug");
	load::parse_hotkey(doc,			variables::misc::eb_key,								"misc",				"eb_key");
	load::parse_bool(doc,			variables::misc::jumpbug,								"misc",				"jumpbug");
	load::parse_hotkey(doc,			variables::misc::jb_key,								"misc",				"jb_key");
	load::parse_bool(doc,			variables::misc::slowwalk,								"misc",				"slowwalk");
	load::parse_hotkey(doc,			variables::misc::slowwalk_key,							"misc",				"slowwalk_key");
	load::parse_multicombo(doc,		variables::misc::speedgraph_target,						"misc",				"speedgraph_target");
	load::parse_float(doc,			variables::misc::speedgraph_h,							"misc",				"speedgraph_h");
	load::parse_float(doc,			variables::misc::speedgraph_pos,						"misc",				"speedgraph_pos");
	load::parse_bool(doc,			variables::misc::backtrack,								"misc",				"backtrack");
	load::parse_bool(doc,			variables::misc::backtrack_team,						"misc",				"backtrack_team");
	load::parse_bool(doc,			variables::misc::thirdperson,							"misc",				"thirdperson");
	load::parse_hotkey(doc,			variables::misc::thirdperson_key,						"misc",				"thirdperson_key");
	load::parse_float(doc,			variables::misc::thirdperson_dist,						"misc",				"thirdperson_dist");
	load::parse_bool(doc,			variables::misc::draw_watermark,						"misc",				"draw_watermark");
	load::parse_bool(doc,			variables::misc::draw_stats,							"misc",				"draw_stats");
	load::parse_bool(doc,			variables::misc::clean_screenshots,						"misc",				"clean_screenshots");
	// Ui
	load::parse_bool(doc,			variables::ui::spectators::spectator_list,				"ui",				"spectator_list");
	// Colors
	load::parse_color(doc,			variables::colors::friendly_color,						"colors",			"friendly_color");
	load::parse_color(doc,			variables::colors::friendly_color_soft,					"colors",			"friendly_color_soft");
	load::parse_color(doc,			variables::colors::friendly_color_softer,				"colors",			"friendly_color_softer");
	load::parse_color(doc,			variables::colors::enemy_color,							"colors",			"enemy_color");
	load::parse_color(doc,			variables::colors::enemy_color_soft,					"colors",			"enemy_color_soft");
	load::parse_color(doc,			variables::colors::enemy_color_softer,					"colors",			"enemy_color_softer");
	load::parse_color(doc,			variables::colors::chams_localplayer,					"colors",			"chams_localplayer");
	load::parse_color(doc,			variables::colors::chams_vis_enemy_c,					"colors",			"chams_vis_enemy_c");
	load::parse_color(doc,			variables::colors::chams_inv_enemy_c,					"colors",			"chams_inv_enemy_c");
	load::parse_color(doc,			variables::colors::chams_vis_friend_c,					"colors",			"chams_vis_friend_c");
	load::parse_color(doc,			variables::colors::chams_inv_friend_c,					"colors",			"chams_inv_friend_c");
	load::parse_color(doc,			variables::colors::chams_weapon_c,						"colors",			"chams_weapon_c");
	load::parse_color(doc,			variables::colors::chams_arms_c,						"colors",			"chams_arms_c");
	load::parse_color(doc,			variables::colors::chams_sleeve_c,						"colors",			"chams_sleeve_c");
	load::parse_color(doc,			variables::colors::bt_chams_enemy,						"colors",			"bt_chams_enemy");
	load::parse_color(doc,			variables::colors::bt_chams_friend,						"colors",			"bt_chams_friend");
	load::parse_color(doc,			variables::colors::bt_chams_enemy_fade,					"colors",			"bt_chams_enemy_fade");
	load::parse_color(doc,			variables::colors::bt_chams_friend_fade,				"colors",			"bt_chams_friend_fade");
	load::parse_color(doc,			variables::colors::crosshair_c,							"colors",			"crosshair_c");
	load::parse_color(doc,			variables::colors::recoil_crosshair_c,					"colors",			"recoil_crosshair_c");
	load::parse_color(doc,			variables::colors::aimbot_fov_c,						"colors",			"aimbot_fov_c");
	load::parse_color(doc,			variables::colors::worldcolor_c,						"colors",			"worldcolor_c");
	// Motion blur
	load::parse_bool(doc,			variables::motion_blur.enabled,							"motion_blur",		"enabled");
	load::parse_bool(doc,			variables::motion_blur.forwardEnabled,					"motion_blur",		"forward_enabled");
	load::parse_float(doc,			variables::motion_blur.fallingMin,						"motion_blur",		"falling_min");
	load::parse_float(doc,			variables::motion_blur.fallingMax,						"motion_blur",		"falling_max");
	load::parse_float(doc,			variables::motion_blur.fallingIntensity,				"motion_blur",		"falling_intensity");
	load::parse_float(doc,			variables::motion_blur.rotationIntensity,				"motion_blur",		"rotation_intensity");
	load::parse_float(doc,			variables::motion_blur.strength,						"motion_blur",		"strength");
}
#pragma endregion

#pragma region LOAD_FUNCTIONS
void config::load::parse_bool(rapidjson::Document& doc, bool& target, std::string parent, std::string json_name) {
	if (!doc.HasMember(parent.c_str())) return;					// Check if parent in doc
	rapidjson::Value& parent_obj = doc[parent.c_str()];			// Get json object from parent
	if (!parent_obj.HasMember(json_name.c_str())) return;		// Check if item in parent
	rapidjson::Value& value = parent_obj[json_name.c_str()];	// Current value inside parent

	if (value.IsBool()) target = value.GetBool();
}

void config::load::parse_float(rapidjson::Document& doc, float& target, std::string parent, std::string json_name) {
	if (!doc.HasMember(parent.c_str())) return;					// Check if parent in doc
	rapidjson::Value& parent_obj = doc[parent.c_str()];			// Get json object from parent
	if (!parent_obj.HasMember(json_name.c_str())) return;		// Check if item in doc
	rapidjson::Value& value = parent_obj[json_name.c_str()];	// value will be each json entry of the item

	if (value.IsFloat()) target = value.GetFloat();
}

void config::load::parse_combobox(rapidjson::Document& doc, combobox_toggle_t& target, std::string parent, std::string json_name) {
	if (!doc.HasMember(parent.c_str())) return;					// Check if parent in doc
	rapidjson::Value& parent_obj = doc[parent.c_str()];			// Get json object from parent
	if (!parent_obj.HasMember(json_name.c_str())) return;		// Check if item in doc
	rapidjson::Value& value = parent_obj[json_name.c_str()];	// value will be each json entry of the item

	if (value.IsInt()) target.idx = value.GetInt();
}

void config::load::parse_multicombo(rapidjson::Document& doc, multicombobox_toggle_t& target, std::string parent, std::string json_name) {
	if (!doc.HasMember(parent.c_str())) return;					// Check if parent in doc
	rapidjson::Value& parent_obj = doc[parent.c_str()];			// Get json object from parent
	if (!parent_obj.HasMember(json_name.c_str())) return;		// Check if item in doc
	rapidjson::Value& value = parent_obj[json_name.c_str()];	// value will be each json entry of the item

	if (value.IsArray()) {											// Check item is array
		for (int n = 0; n < target.vector.size(); n++) {			// Each target vector item
			if (n < value.Size()) {									// Json array is good
				if (value[n].IsBool())								// Check json value is a bool
					target.vector[n].state = value[n].GetBool();	// Assign to target
			}
		}
	}
}

void config::load::parse_color(rapidjson::Document& doc, colorpicker_col_t& target, std::string parent, std::string json_name) {
	if (!doc.HasMember(parent.c_str())) return;											// Check if parent in doc
	rapidjson::Value& parent_obj = doc[parent.c_str()];									// Get json object from parent
	if (!parent_obj.IsObject() || !parent_obj.HasMember(json_name.c_str())) return;		// Check if item in doc
	rapidjson::Value& color_obj = parent_obj[json_name.c_str()];						// color_obj will be the obj containing the rgb and hsv arrays
	if (!color_obj.IsObject()) return;													// Check if the color object is an object
	
	if (color_obj.HasMember("rgb")) {								// Get rgb array from color obj
		rapidjson::Value& rgb_arr = color_obj["rgb"];				// rgb_arr will be the array containing the rgb integers

		if (rgb_arr.IsArray()) {									// Check item is array
			if (rgb_arr.Size() > 0 && rgb_arr[0].IsInt())			// Json array is good and json value is an int
				target.col.r = rgb_arr[0].GetInt();					// (R) Assign to color parameter
			if (rgb_arr.Size() > 1 && rgb_arr[1].IsInt())
				target.col.g = rgb_arr[1].GetInt();					// (G)
			if (rgb_arr.Size() > 2 && rgb_arr[2].IsInt())
				target.col.b = rgb_arr[2].GetInt();					// (B)
			if (rgb_arr.Size() > 3 && rgb_arr[3].IsInt())
				target.col.a = rgb_arr[3].GetInt();					// (A)
		}
	}

	if (color_obj.HasMember("hsv")) {							// Get hsv array from color obj
		rapidjson::Value& hsv_arr = color_obj["hsv"];			// hsv_arr will be the array containing the hsv floats

		if (hsv_arr.IsArray()) {								// Check item is array
			if (hsv_arr.Size() > 0) {							// Json array is good and json value is float (float_hsv)
				if (hsv_arr[0].IsFloat())		target.f_hsv.h = hsv_arr[0].GetFloat();		// (H) Assign to float_hsv parameter
				else if (hsv_arr[0].IsInt())	target.f_hsv.h = hsv_arr[0].GetInt();		// Int check just in case
			}
			if (hsv_arr.Size() > 1) {
				if (hsv_arr[1].IsFloat())		target.f_hsv.s = hsv_arr[1].GetFloat();		// (S)
				else if (hsv_arr[1].IsInt())	target.f_hsv.s = hsv_arr[1].GetInt();
			}
			if (hsv_arr.Size() > 2) {
				if (hsv_arr[2].IsFloat())		target.f_hsv.v = hsv_arr[2].GetFloat();		// (V)
				else if (hsv_arr[2].IsInt())	target.f_hsv.v = hsv_arr[2].GetInt();
			}
		}
	}
}

void config::load::parse_hotkey(rapidjson::Document& doc, hotkey_t& target, std::string parent, std::string json_name) {
	if (!doc.HasMember(parent.c_str())) return;					// Check if parent in doc
	rapidjson::Value& parent_obj = doc[parent.c_str()];			// Get json object from parent
	if (!parent_obj.HasMember(json_name.c_str())) return;		// Check if item in doc
	rapidjson::Value& value = parent_obj[json_name.c_str()];	// value will be each json entry of the item

	if (value.IsInt()) target.key = value.GetInt();
}
#pragma endregion
