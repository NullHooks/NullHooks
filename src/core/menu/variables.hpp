#pragma once

namespace variables {
	
	namespace aim {
		inline bool aimbot = false;
		inline float aimbot_fov = 20.f;
		inline bool draw_fov = false;
		inline float aimbot_smoothing = 0.5f;
		inline bool aimbot_isvisiblecheck = true;
		inline bool aimbot_noscope = true;
	}

	namespace player_visuals {
		inline bool showteamesp = false;
		inline bool playerglow = false;
		inline bool boxesp = false;
		inline bool lineesp = false;
		inline bool skeletonesp = false;
		inline bool nameesp = false;
		inline bool playerinfo = false;
		inline bool healthesp = false;
	}

	namespace entity_visuals {
		inline bool entityglow = false;
		inline bool entitytext = false;
		inline bool bombtimer = true;
		inline bool nade_esp = false;
	}

	namespace chams {
		inline bool only_visible_chams = false;
		inline bool draw_chams_on_top = false;
		inline bool wireframe_chams = false;
		inline bool player_chams = false;
		inline bool vm_weapon_chams = false;
		inline bool vm_arm_chams = false;
		inline bool vm_sleeve_chams = false;

		inline int player_chams_mat_id = 1;
		inline int sleeve_chams_mat_id = 10;
		inline int arm_chams_mat_id = 5;
		inline int weapon_chams_mat_id = 9;
	}

	namespace misc_visuals {
		inline bool nade_predict = true;
		inline bool noflash = true;
		inline bool noscope  = true;
		inline bool crosshair = false;
		inline bool recoil_crosshair = false;

		inline bool chickenpride = false;

		inline float custom_fov_slider = 90.f;
		inline float custom_vmfov_slider = 1.f;
	}

	namespace misc {
		inline bool bhop = true;
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
	
	namespace colors {
		// Esp and glow
		inline color friendly_color = color(0, 72, 255, 255);
		inline color friendly_color_soft = color(0, 150, 255, 255);
		inline color friendly_color_softer = color(90, 180, 255, 255);
		inline color enemy_color = color::red(255);
		inline color enemy_color_soft = color(190, 25, 25, 255);
		inline color enemy_color_softer = color(255, 75, 75, 255);

		// Chams
		inline color chams_inv_enemy_c = color(150, 15, 15, 255);
		inline color chams_vis_enemy_c = color(230, 20, 70);
		inline color chams_inv_friend_c = color(0, 75, 255);
		inline color chams_vis_friend_c = color(0, 150, 255);

		inline color chams_sleeve_c = color(200, 0, 200, 255);
		inline color chams_arms_c = color(255, 0, 255, 255);
		inline color chams_weapon_c = color(255, 150, 255, 255);
	}

}
