#pragma once

namespace variables {
	
	// Aim
	inline bool aimbot_bool = false;
	inline float aimbot_fov = 0.f;
	inline float aimbot_smoothing = 2.f;
	inline bool aimbot_isvisiblecheck = true;

	// Visuals - Player ESP
	inline bool showteamesp_bool = false;
	inline bool playerglow_bool = false;
	inline bool boxesp_bool = false;
	inline bool lineesp_bool = false;
	inline bool skeletonesp_bool = false;
	inline bool nameesp_bool = false;
	inline bool playerinfo_bool = false;
	inline bool healthesp_bool = false;

	// Visuals - Entity ESP
	inline bool entityglow_bool = false;
	inline bool entitytext_bool = false;
	inline bool bombtimer_bool = true;
	inline bool nade_esp_bool = false;
	
	// Visuals - Chams
	inline bool player_chams_bool = false;
	inline bool only_visible_chams_bool = false;
	inline bool vm_weapon_chams_bool = false;
	inline bool vm_arm_chams_bool = false;
	inline bool vm_sleeve_chams_bool = false;

	inline int player_chams_mat_id = 1;
	inline int sleeve_chams_mat_id = 8;
	inline int arm_chams_mat_id = 5;
	inline int weapon_chams_mat_id = 9;

	// Visuals - Misc
	inline bool nade_predict_bool = true;
	inline bool noflash_bool = true;
	inline bool noscope_bool  = true;
	inline bool crosshair_bool = false;
	inline bool recoil_crosshair_bool = false;
	inline bool clean_screenshots_bool = true;

	inline float custom_fov_slider = 90.f;
	inline float custom_vmfov_slider = 1.f;

	// Colors
	namespace colors {
		// Esp and glow
		inline color friendly_color = color(0, 72, 255, 255);
		inline color friendly_color_soft = color(0, 150, 255, 255);	// color(85, 235, 255, 255)
		inline color friendly_color_softer = color(90, 180, 255, 255);
		inline color enemy_color = color::red(255);
		inline color enemy_color_soft = color(190, 25, 25, 255);
		inline color enemy_color_softer = color(255, 75, 75, 255);

		// Chams
		inline color chams_inv_enemy_c = color(150, 15, 15, 255);
		inline color chams_vis_enemy_c = color(230, 20, 70);
		inline color chams_inv_friend_c = color(0, 75, 255);
		inline color chams_vis_friend_c = color(0, 150, 255);

		inline color chams_sleeve_c = color(200, 0, 200, 255);	// Old: color(200, 0, 200, 200)
		inline color chams_arms_c = color(255, 0, 255, 255);
		inline color chams_weapon_c = color(255, 150, 255, 255);
	};

	// Misc
	inline bool bhop_bool = true;
	inline bool chickenpride_bool = false;
	inline bool draw_watermark = true;
	inline bool draw_stats = true;
	
	namespace spectators {
		inline bool spectator_list_bool = true;
		inline int x = 10, y = 560;
		inline int w = 100, h = 10;
	};

	namespace crosshair {
		const inline bool only_engine_crosshair = true;		// Edit here
		inline bool using_cs_crosshair = true;

		inline float crosshair_len = 3;		// 2-20
		inline float crosshair_w = 2;		// 2-15
		inline float crosshair_gap = 0;		// 0-10

		inline bool rainbow_crosshair = false;
	};

	// Menu
	namespace menu {
		inline bool opened = false;
		inline int x = 300, y = 200;
		inline int w = 500, h = 430;
	};

	namespace watermark {
		inline int y = 10;
		inline int x = 15;
	};
}
