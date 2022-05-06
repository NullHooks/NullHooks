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

	// Visuals - Glow
	inline bool entityglow_bool = false;
	inline bool entitytext_bool = false;
	inline bool bombtimer_bool = true;
	inline bool nade_esp_bool = false;
	
	// Visuals - Misc
	inline bool nade_predict_bool = true;
	inline bool noflash_bool = true;
	inline bool noscope_bool  = true;
	inline bool crosshair_bool = false;
	inline bool recoil_crosshair_bool = false;
	inline float custom_fov_slider = 90.f;
	inline float custom_vmfov_slider = 1.f;

	// Colors
	namespace colors {
		inline color friendly_color = color(0, 72, 255, 255);
		inline color friendly_color_soft = color(0, 150, 255, 255);	// color(85, 235, 255, 255)
		inline color friendly_color_softer = color(90, 180, 255, 255);
		inline color enemy_color = color::red(255);
		inline color enemy_color_soft = color(190, 25, 25, 255);
		inline color enemy_color_softer = color(255, 75, 75, 255);
	};

	// Misc
	inline bool bhop_bool = true;
	inline bool chickenpride_bool = false;
	
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
		inline int w = 340, h = 400;
	};

	namespace watermark {
		inline int y = 10;
		inline int x = 15;
	};
}