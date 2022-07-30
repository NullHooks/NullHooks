#include "dependencies/utilities/csgo.hpp"
#include "core/config/config.hpp"
#include "core/menu/variables.hpp"
#include "core/features/visuals/skin_changer/skin_changer.hpp"
// Rapidjson
#include "dependencies/rapidjson/document.h"
#include "dependencies/rapidjson/writer.h"
#include "dependencies/rapidjson/stringbuffer.h"

void config::save_selected_config() {
	if (selected_config >= 0 && selected_config < config_names.size())
		save_config(config_names.at(selected_config));
}

#pragma region SAVE_CONFIG
void config::create_new_config(std::string filename) {
	save_config(filename);
	refresh_list();
}

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
	rapidjson::Value aim(rapidjson::kObjectType); {					// Aim
		save::parse_bool(aim,					allocator,			variables::aim::triggerbot,								"triggerbot");
		save::parse_hotkey(aim,					allocator,			variables::aim::triggerbot_key,							"triggerbot_key");
		save::parse_float(aim,					allocator,			variables::aim::triggerbot_delay,						"triggerbot_delay");
		save::parse_bool(aim,					allocator,			variables::aim::aimbot,									"aimbot");
		save::parse_bool(aim,					allocator,			variables::aim::silent,									"silent");
		save::parse_bool(aim,					allocator,			variables::aim::autofire,								"autofire");
		save::parse_hotkey(aim,					allocator,			variables::aim::aimbot_key,								"aimbot_key");
		save::parse_bool(aim,					allocator,			variables::aim::bodyaim_if_lethal,						"bodyaim_if_lethal");
		save::parse_combobox(aim,				allocator,			variables::aim::autowall,								"autowall");
		save::parse_float(aim,					allocator,			variables::aim::aimbot_fov,								"aimbot_fov");
		save::parse_bool(aim,					allocator,			variables::aim::draw_fov,								"draw_fov");
		save::parse_float(aim,					allocator,			variables::aim::aimbot_smoothing,						"aimbot_smoothing");
		save::parse_bool(aim,					allocator,			variables::aim::target_friends,							"target_friends");
		save::parse_bool(aim,					allocator,			variables::aim::non_rifle_aimpunch,						"non_rifle_aimpunch");
		save::parse_bool(aim,					allocator,			variables::aim::aimbot_noscope,							"aimbot_noscope");
		save::parse_float(aim,					allocator,			variables::aim::min_damage,								"min_damage");
		save::parse_multicombo(aim,				allocator,			variables::aim::hitboxes,								"hitboxes");
	} doc.AddMember("aim", aim, allocator);
	
	rapidjson::Value antiaim(rapidjson::kObjectType); {				// Antiaim
		save::parse_bool(antiaim,				allocator,			variables::antiaim::antiaim,							"antiaim");
		save::parse_float(antiaim,				allocator,			variables::antiaim::pitch,								"antiaim_pitch");
		save::parse_float(antiaim,				allocator,			variables::antiaim::yaw,								"antiaim_yaw");
		save::parse_bool(antiaim,				allocator,			variables::antiaim::spinbot,							"spinbot");
		save::parse_float(antiaim,				allocator,			variables::antiaim::spinbot_speed,						"spinbot_speed");
		save::parse_bool(antiaim,				allocator,			variables::antiaim::peek_aa,							"peek_aa");
		save::parse_hotkey(antiaim,				allocator,			variables::antiaim::peek_aa_toggle_key,					"peek_aa_toggle_key");
	} doc.AddMember("antiaim", antiaim, allocator);

	rapidjson::Value player_visuals(rapidjson::kObjectType); {		// Player visuals
		save::parse_bool(player_visuals,		allocator,			variables::player_visuals::showteamesp,					"showteamesp");
		save::parse_bool(player_visuals,		allocator,			variables::player_visuals::playerglow,					"playerglow");
		save::parse_bool(player_visuals,		allocator,			variables::player_visuals::boxesp,						"boxesp");
		save::parse_bool(player_visuals,		allocator,			variables::player_visuals::lineesp,						"lineesp");
		save::parse_bool(player_visuals,		allocator,			variables::player_visuals::skeletonesp,					"skeletonesp");
		save::parse_bool(player_visuals,		allocator,			variables::player_visuals::nameesp,						"nameesp");
		save::parse_multicombo(player_visuals,	allocator,			variables::player_visuals::playerinfo,					"playerinfo");
		save::parse_bool(player_visuals,		allocator,			variables::player_visuals::healthesp,					"healthesp");
	} doc.AddMember("player_visuals", player_visuals, allocator);
	
	rapidjson::Value entity_visuals(rapidjson::kObjectType); {		// Entity visuals
		save::parse_bool(entity_visuals,		allocator,			variables::entity_visuals::entityglow,					"entityglow");
		save::parse_bool(entity_visuals,		allocator,			variables::entity_visuals::entitytext,					"entitytext");
		save::parse_bool(entity_visuals,		allocator,			variables::entity_visuals::bombtimer,					"bombtimer");
		save::parse_bool(entity_visuals,		allocator,			variables::entity_visuals::nade_esp,					"nade_esp");
	} doc.AddMember("entity_visuals", entity_visuals, allocator);
	
	rapidjson::Value chams(rapidjson::kObjectType); {				// Chams
		save::parse_bool(chams,					allocator,			variables::chams::only_visible_chams,					"only_visible_chams");
		save::parse_bool(chams,					allocator,			variables::chams::draw_chams_on_top,					"draw_chams_on_top");
		save::parse_bool(chams,					allocator,			variables::chams::wireframe_chams,						"wireframe_chams");
		save::parse_bool(chams,					allocator,			variables::chams::player_chams,							"player_chams");
		save::parse_bool(chams,					allocator,			variables::chams::localplayer_chams,					"localplayer_chams");
		save::parse_bool(chams,					allocator,			variables::chams::vm_weapon_chams,						"vm_weapon_chams");
		save::parse_bool(chams,					allocator,			variables::chams::vm_arm_chams,							"vm_arm_chams");
		save::parse_bool(chams,					allocator,			variables::chams::vm_sleeve_chams,						"vm_sleeve_chams");
		save::parse_combobox(chams,				allocator,			variables::chams::player_chams_mat_id,					"player_mat_id");
		save::parse_combobox(chams,				allocator,			variables::chams::localplayer_chams_mat_id,				"localplayer_mat_id");
		save::parse_combobox(chams,				allocator,			variables::chams::weapon_chams_mat_id,					"weapon_mat_id");
		save::parse_combobox(chams,				allocator,			variables::chams::arm_chams_mat_id,						"arms_mat_id");
		save::parse_combobox(chams,				allocator,			variables::chams::sleeve_chams_mat_id,					"sleeve_mat_id");
		save::parse_bool(chams,					allocator,			variables::chams::backtrack_chams,						"backtrack_chams");
	} doc.AddMember("chams", chams, allocator);
	
	rapidjson::Value misc_visuals(rapidjson::kObjectType); {		// Misc visuals
		save::parse_float(misc_visuals,			allocator,			variables::misc_visuals::noflash_alpha,					"noflash");
		save::parse_bool(misc_visuals,			allocator,			variables::misc_visuals::wireframe_smoke,				"wireframe_smoke");
		save::parse_bool(misc_visuals,			allocator,			variables::misc_visuals::nade_predict,					"nade_predict");
		save::parse_bool(misc_visuals,			allocator,			variables::misc_visuals::noscope,						"noscope");
		save::parse_bool(misc_visuals,			allocator,			variables::misc_visuals::crosshair,						"crosshair");
		save::parse_bool(misc_visuals,			allocator,			variables::misc_visuals::recoil_crosshair,				"recoil_crosshair");
		save::parse_multicombo(misc_visuals,	allocator,			variables::misc_visuals::bulletracer_draw_target,		"bulletracer_draw_target");
		save::parse_multicombo(misc_visuals,	allocator,			variables::misc_visuals::bulletracer_team_target,		"bulletracer_team_target");
		save::parse_bool(misc_visuals,			allocator,			variables::misc_visuals::chickenpride,					"chickenpride");
		save::parse_float(misc_visuals,			allocator,			variables::misc_visuals::custom_fov_slider,				"custom_fov_slider");
		save::parse_float(misc_visuals,			allocator,			variables::misc_visuals::custom_vmfov_slider,			"custom_vmfov_slider");
		save::parse_bool(misc_visuals,			allocator,			variables::misc_visuals::worldcolor,					"worldcolor");
	} doc.AddMember("misc_visuals", misc_visuals, allocator);
	
	rapidjson::Value misc(rapidjson::kObjectType); {				// Misc
		save::parse_bool(misc,					allocator,			variables::misc::infinite_duck,							"infinite_duck");
		save::parse_bool(misc,					allocator,			variables::misc::bhop,									"bhop");
		save::parse_combobox(misc,				allocator,			variables::misc::autostrafe_target,						"autostrafe");
		save::parse_bool(misc,					allocator,			variables::misc::edgejump,								"edgejump");
		save::parse_hotkey(misc,				allocator,			variables::misc::ej_key,								"ej_key");
		save::parse_bool(misc,					allocator,			variables::misc::edgebug,								"edgebug");
		save::parse_hotkey(misc,				allocator,			variables::misc::eb_key,								"eb_key");
		save::parse_bool(misc,					allocator,			variables::misc::jumpbug,								"jumpbug");
		save::parse_hotkey(misc,				allocator,			variables::misc::jb_key,								"jb_key");
		save::parse_bool(misc,					allocator,			variables::misc::slowwalk,								"slowwalk");
		save::parse_hotkey(misc,				allocator,			variables::misc::slowwalk_key,							"slowwalk_key");
		save::parse_multicombo(misc,			allocator,			variables::misc::speedgraph_target,						"speedgraph_target");
		save::parse_float(misc,					allocator,			variables::misc::speedgraph_h,							"speedgraph_h");
		save::parse_float(misc,					allocator,			variables::misc::speedgraph_pos,						"speedgraph_pos");
		save::parse_bool(misc,					allocator,			variables::misc::backtrack,								"backtrack");
		save::parse_bool(misc,					allocator,			variables::misc::backtrack_team,						"backtrack_team");
		save::parse_bool(misc,					allocator,			variables::misc::thirdperson,							"thirdperson");
		save::parse_hotkey(misc,				allocator,			variables::misc::thirdperson_key,						"thirdperson_key");
		save::parse_float(misc,					allocator,			variables::misc::thirdperson_dist,						"thirdperson_dist");
		save::parse_bool(misc,					allocator,			variables::misc::draw_watermark,						"draw_watermark");
		save::parse_bool(misc,					allocator,			variables::misc::draw_stats,							"draw_stats");
		save::parse_bool(misc,					allocator,			variables::misc::clean_screenshots,						"clean_screenshots");
	} doc.AddMember("misc", misc, allocator);
	
	rapidjson::Value ui(rapidjson::kObjectType); {					// Ui
		save::parse_bool(ui,					allocator,			variables::ui::spectators::spectator_list,				"spectator_list");
	} doc.AddMember("ui", ui, allocator);
	
	rapidjson::Value colors(rapidjson::kObjectType); {				// Colors
		save::parse_color(colors,				allocator,			variables::colors::friendly_color,						"friendly_color");
		save::parse_color(colors,				allocator,			variables::colors::friendly_color_soft,					"friendly_color_soft");
		save::parse_color(colors,				allocator,			variables::colors::friendly_color_softer,				"friendly_color_softer");
		save::parse_color(colors,				allocator,			variables::colors::enemy_color,							"enemy_color");
		save::parse_color(colors,				allocator,			variables::colors::enemy_color_soft,					"enemy_color_soft");
		save::parse_color(colors,				allocator,			variables::colors::enemy_color_softer,					"enemy_color_softer");
		save::parse_color(colors,				allocator,			variables::colors::enemy_color_softer,					"enemy_color_softer");
		save::parse_color(colors,				allocator,			variables::colors::chams_localplayer,					"chams_localplayer");
		save::parse_color(colors,				allocator,			variables::colors::chams_vis_enemy_c,					"chams_vis_enemy_c");
		save::parse_color(colors,				allocator,			variables::colors::chams_inv_enemy_c,					"chams_inv_enemy_c");
		save::parse_color(colors,				allocator,			variables::colors::chams_vis_friend_c,					"chams_vis_friend_c");
		save::parse_color(colors,				allocator,			variables::colors::chams_inv_friend_c,					"chams_inv_friend_c");
		save::parse_color(colors,				allocator,			variables::colors::chams_weapon_c,						"chams_weapon_c");
		save::parse_color(colors,				allocator,			variables::colors::chams_arms_c,						"chams_arms_c");
		save::parse_color(colors,				allocator,			variables::colors::chams_sleeve_c,						"chams_sleeve_c");
		save::parse_color(colors,				allocator,			variables::colors::bt_chams_enemy,						"bt_chams_enemy");
		save::parse_color(colors,				allocator,			variables::colors::bt_chams_friend,						"bt_chams_friend");
		save::parse_color(colors,				allocator,			variables::colors::bt_chams_enemy_fade,					"bt_chams_enemy_fade");
		save::parse_color(colors,				allocator,			variables::colors::bt_chams_friend_fade,				"bt_chams_friend_fade");
		save::parse_color(colors,				allocator,			variables::colors::crosshair_c,							"crosshair_c");
		save::parse_color(colors,				allocator,			variables::colors::recoil_crosshair_c,					"recoil_crosshair_c");
		save::parse_color(colors,				allocator,			variables::colors::aimbot_fov_c,						"aimbot_fov_c");
		save::parse_color(colors,				allocator,			variables::colors::worldcolor_c,						"worldcolor_c");
	} doc.AddMember("colors", colors, allocator);
	
	rapidjson::Value motion_blur(rapidjson::kObjectType); {			// Motion blur
		save::parse_bool(motion_blur,			allocator,			variables::motion_blur.enabled,							"enabled");
		save::parse_bool(motion_blur,			allocator,			variables::motion_blur.forwardEnabled,					"forward_enabled");
		save::parse_float(motion_blur,			allocator,			variables::motion_blur.fallingMin,						"falling_min");
		save::parse_float(motion_blur,			allocator,			variables::motion_blur.fallingMax,						"falling_max");
		save::parse_float(motion_blur,			allocator,			variables::motion_blur.fallingIntensity,				"falling_intensity");
		save::parse_float(motion_blur,			allocator,			variables::motion_blur.rotationIntensity,				"rotation_intensity");
		save::parse_float(motion_blur,			allocator,			variables::motion_blur.strength,						"strength");
	} doc.AddMember("motion_blur", motion_blur, allocator);
	
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
#pragma endregion

#pragma region SAVE_FUNCTIONS
void config::save::parse_bool(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator, bool& target, std::string json_name) {
	rapidjson::Value name(json_name.c_str(), allocator);		// Name used by rapidjson for the item ("name": 123)
	parent.AddMember(name, target, allocator);
}

void config::save::parse_float(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator, float& target, std::string json_name) {
	rapidjson::Value name(json_name.c_str(), allocator);
	parent.AddMember(name, target, allocator);
}

void config::save::parse_combobox(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator, combobox_toggle_t& target, std::string json_name) {
	rapidjson::Value name(json_name.c_str(), allocator);
	parent.AddMember(name, target.idx, allocator);
}

void config::save::parse_multicombo(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator, multicombobox_toggle_t& target, std::string json_name) {
	rapidjson::Value name(json_name.c_str(), allocator);
	rapidjson::Value arr(rapidjson::kArrayType);

	for (int n = 0; n < target.vector.size(); n++) {			// Each target vector item
		arr.PushBack(target.vector.at(n).state, allocator);		// Add item to array
	}

	// Add array to main doc
	parent.AddMember(name, arr, allocator);
}

void config::save::parse_color(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator, colorpicker_col_t& target, std::string json_name) {
	rapidjson::Value name(json_name.c_str(), allocator);	// Name used by rapidjson for the item ("name": 123)
	rapidjson::Value col_obj(rapidjson::kObjectType);		// The color object containing the rgb and the hsv arrays
	rapidjson::Value rgb_arr(rapidjson::kArrayType);
	rapidjson::Value hsv_arr(rapidjson::kArrayType);

	// Add colors to rgb array in order
	rgb_arr.PushBack(target.col.r, allocator);
	rgb_arr.PushBack(target.col.g, allocator);
	rgb_arr.PushBack(target.col.b, allocator);
	rgb_arr.PushBack(target.col.a, allocator);

	// Add colors to hsv array in order
	hsv_arr.PushBack(target.f_hsv.h, allocator);
	hsv_arr.PushBack(target.f_hsv.s, allocator);
	hsv_arr.PushBack(target.f_hsv.v, allocator);

	// Add arrays to color object
	col_obj.AddMember("rgb", rgb_arr, allocator);
	col_obj.AddMember("hsv", hsv_arr, allocator);

	// Add array to main doc
	parent.AddMember(name, col_obj, allocator);
}

void config::save::parse_hotkey(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator, hotkey_t& target, std::string json_name) {
	rapidjson::Value name(json_name.c_str(), allocator);
	parent.AddMember(name, target.key, allocator);
}
#pragma endregion
