#pragma once
#include "core/menu/global_input.hpp"
#include "core/menu/framework.hpp"

namespace variables {
	namespace aim {
		inline bool triggerbot = false;
		inline hotkey_t triggerbot_key(VK_PRIOR);
		inline float triggerbot_delay = 0.f;

		inline bool aimbot = false;
		inline bool silent = false;
		inline bool autofire = false;
		inline hotkey_t aimbot_key(VK_NEXT);
		inline bool bodyaim_if_lethal = false;
		
		inline std::vector<std::string> autowall_settings = {
			"Only visible",
			"Autowall",
			"Ignore walls"
		};
		inline combobox_toggle_t autowall(0);	// Will store the autowall setting: 0 only visible, 1 autowall, 2 ignore walls
		
		inline float aimbot_fov = 20.f;
		inline bool draw_fov = false;

		inline float aimbot_smoothing = 0.f;
		inline bool target_friends = false;
		inline bool non_rifle_aimpunch = true;
		inline bool aimbot_noscope = true;

		inline float min_damage = 60.f;

		inline std::vector<multicombo_opt_t> hitboxes_options = {
			{ "Head",	        true },
			{ "Neck",	        true },
			{ "Chest",			false },
			{ "Arms",			false },
			{ "Legs",			false },
		};
		inline multicombobox_toggle_t hitboxes(hitboxes_options);
	}

	namespace antiaim {
		inline bool antiaim = false;
		inline float yaw = 0.f;
		inline float pitch = 0.f;
		inline bool spinbot = false;
		inline float spinbot_speed = 0.f;
		inline bool peek_aa = false;
		inline hotkey_t peek_aa_toggle_key(VK_XBUTTON1);
	}

	namespace player_visuals {
		inline bool showteamesp = false;
		inline bool playerglow = false;
		inline bool boxesp = false;
		inline bool lineesp = false;
		inline bool skeletonesp = false;
		inline bool nameesp = false;

		inline std::vector<multicombo_opt_t> playerinfo_options = {
			{ "Weapon",			false },
			{ "Armor",			false },
			{ "Defuser",		false },
			{ "Bomb",			false },
			{ "Scoped",			false },
			{ "Flashed",		false },
			{ "Can fire",		false }
		};
		inline multicombobox_toggle_t playerinfo(playerinfo_options);

		inline bool healthesp = false;
	}

	namespace entity_visuals {
		inline bool entityglow = false;
		inline bool entitytext = false;
		inline bool bombtimer = true;
		inline bool nade_esp = false;
	}

	namespace chams {
		inline std::vector<std::string> materials = {
			"Transparent",
			"Textured",
			"Flat",
			"Ghost",
			"Gold 1",
			"Gold 2",
			"Amethyst",
			"Silver",
			"Steel",
			"Dark steel",
			"Plastic",
			"Red glow",
			"Effect 1",
			"Effect 2",
			"Effect 3",
			"Animated blink",
			"Animated stripes"
		};

		// Enabled options
		inline bool only_visible_chams = false;
		inline bool draw_chams_on_top = false;
		inline bool wireframe_chams = false;
		inline bool player_chams = false;
		inline bool localplayer_chams = false;
		inline bool vm_weapon_chams = false;
		inline bool vm_arm_chams = false;
		inline bool vm_sleeve_chams = false;

		// Selected material for each setting
		inline combobox_toggle_t player_chams_mat_id(1);
		inline combobox_toggle_t localplayer_chams_mat_id(1);
		inline combobox_toggle_t weapon_chams_mat_id(4);
		inline combobox_toggle_t arm_chams_mat_id(1);
		inline combobox_toggle_t sleeve_chams_mat_id(1);

		// Backtrack
		inline bool backtrack_chams = true;
	}

	namespace misc_visuals {
		inline float noflash_alpha = 0.f;
		inline bool wireframe_smoke = false;
		inline bool nade_predict = true;
		inline bool noscope  = true;
		inline bool crosshair = false;
		inline bool recoil_crosshair = false;
		inline bool worldcolor = false;

		inline std::vector<multicombo_opt_t> bulletracer_draw_options = {
			{ "Line",			false },
			{ "Impacts",		false }
			//{ "Hits",			false }		// Will just draw a different color for hits
		};
		inline std::vector<multicombo_opt_t> bulletracer_team_options = {
			{ "Localplayer",	true },
			{ "Friendly",		false },
			{ "Enemy",			false }
		};
		inline multicombobox_toggle_t bulletracer_draw_target(bulletracer_draw_options);
		inline multicombobox_toggle_t bulletracer_team_target(bulletracer_team_options);

		inline bool chickenpride = false;

		inline float custom_fov_slider = 90.f;
		inline float custom_vmfov_slider = 1.f;
	}

	namespace misc {
		// Movement
		inline bool infinite_duck = false;
		inline bool bhop = true;

		inline std::vector<std::string> autostrafe_options = {
			"None",
			"Legit",
			"Rage"
		};
		inline combobox_toggle_t autostrafe_target(0);
		
		inline bool edgejump = false;
		inline hotkey_t ej_key('C');	// Mind the ' ' and the capital letter
		inline bool edgebug = false;
		inline hotkey_t eb_key(VK_XBUTTON1);
		inline bool jumpbug = false;
		inline hotkey_t jb_key(VK_MENU);		// Alt key
		inline bool slowwalk = false;
		inline hotkey_t slowwalk_key(VK_MENU);	// Alt key

		inline std::vector<multicombo_opt_t> speedgraph_options = {
			{ "Line",		false },
			{ "Color",		false },
			{ "Speed",		true },
			{ "Jump",		true }
		};
		inline multicombobox_toggle_t speedgraph_target(speedgraph_options);
		inline float speedgraph_h = 40.f;
		inline float speedgraph_pos = 92.5;
		
		// Network
		inline bool backtrack = false;
		inline bool backtrack_team = false;
		inline float backtrack_ticks = 200.f;

		// Thirdperson
		inline bool thirdperson = false;
		inline hotkey_t thirdperson_key('P');
		inline float thirdperson_dist = 100.f;

		// UI
		inline bool draw_watermark = true;
		inline bool draw_stats = true;
		inline bool clean_screenshots = true;
	}
	
	namespace ui {
		namespace spectators {
			inline bool spectator_list = true;
			inline int x = 10, y = 560;
			inline int w = 100, h = 10;
		}

		namespace menu {
			inline bool opened = false;
			inline int x = 300, y = 200;
			inline int w = 500, h = 450;
		}

		namespace watermark {
			inline int y = 10;
			inline int x = 15;
		}
	}

	namespace crosshair {
		const inline bool only_engine_crosshair = true;		// Edit here
		inline bool using_cs_crosshair = true;

		inline float crosshair_len = 3;		// 2-20
		inline float crosshair_w = 2;		// 2-15
		inline float crosshair_gap = 0;		// 0-10

		inline bool rainbow_crosshair = false;
	}

	// Despite the indentation, we are calling constructor with color as argument
	namespace colors {
		// Esp and glow
		inline colorpicker_col_t friendly_color			(color(0, 90, 255, 255));
		inline colorpicker_col_t friendly_color_soft	(color(0, 150, 255, 255));
		inline colorpicker_col_t friendly_color_softer	(color(90, 180, 255, 255));
		inline colorpicker_col_t enemy_color			(color(255, 0, 0, 255));
		inline colorpicker_col_t enemy_color_soft		(color(190, 25, 25, 255));
		inline colorpicker_col_t enemy_color_softer		(color(255, 75, 75, 255));

		inline colorpicker_col_t friendly_glow_c		(friendly_color_soft.col.get_custom_alpha(200));
		inline colorpicker_col_t enemy_glow_c			(enemy_color_soft.col.get_custom_alpha(200));
		inline colorpicker_col_t entity_glow_c			(color::white(200));

		// Chams
		inline colorpicker_col_t chams_localplayer		(color(255, 150, 255, 255));
		inline colorpicker_col_t chams_vis_enemy_c		(color(230, 20, 70));
		inline colorpicker_col_t chams_inv_enemy_c		(color(150, 15, 15, 255));
		inline colorpicker_col_t chams_vis_friend_c		(color(0, 150, 255));
		inline colorpicker_col_t chams_inv_friend_c		(color(0, 75, 255));

		inline colorpicker_col_t chams_weapon_c			(color(255, 150, 255, 255));
		inline colorpicker_col_t chams_arms_c			(color(255, 0, 255, 255));
		inline colorpicker_col_t chams_sleeve_c			(color(100, 0, 255, 255));

		inline colorpicker_col_t bt_chams_enemy			(chams_vis_enemy_c.col.get_custom_alpha(30));		// Backtrack enemy
		inline colorpicker_col_t bt_chams_friend		(chams_vis_friend_c.col.get_custom_alpha(30));		// Backtrack friendly
		inline colorpicker_col_t bt_chams_enemy_fade	(color::white(200));		// Used for the color transition
		inline colorpicker_col_t bt_chams_friend_fade	(color::white(200));		// Used for the color transition

		// Misc
		inline colorpicker_col_t crosshair_c			(color(255, 255, 255, 255));
		inline colorpicker_col_t recoil_crosshair_c		(color(0, 255, 0, 255));
		inline colorpicker_col_t aimbot_fov_c			(color(255, 255, 255, 150));
		inline colorpicker_col_t worldcolor_c			(color(125, 0, 170, 255));
	};

	inline struct MotionBlur {
		bool enabled{ false };
		bool forwardEnabled{ false };
		float fallingMin{ 10.0f };
		float fallingMax{ 20.0f };
		float fallingIntensity{ 1.0f };
		float rotationIntensity{ 1.0f };
		float strength{ 4.0f };
	} motion_blur;
}
