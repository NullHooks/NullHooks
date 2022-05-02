#pragma once

namespace variables {
	
	// Legitbot
	inline bool aimbot_bool = false;
	inline float aimbot_fov = 0.f;
	inline float aimbot_smoothing = 2.f;
	inline bool aimbot_isvisiblecheck = true;

	// Visuals
	inline bool showteamesp_bool = false;
	inline bool boxesp_bool = false;
	inline bool skeletonesp_bool = false;
	inline bool nameesp_bool = false;
	inline bool healthesp_bool = false;
	inline bool nade_esp_bool = false;

	inline bool nade_predict_bool = true;
	inline bool noflash_bool = true;
	inline bool noscope_bool  = true;
	inline bool crosshair_bool = false;
	inline bool recoil_crosshair_bool = false;

	// Misc
	inline bool bhop_bool = true;

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

	namespace menu {
		inline bool opened = false;
		inline int x = 300, y = 200;
		inline int w = 350, h = 300;
	};

	namespace watermark {
		inline int y = 10;
		inline int x = 15;
	};
}