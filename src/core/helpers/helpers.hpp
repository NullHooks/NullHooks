#pragma once
#include "dependencies/utilities/csgo.hpp"

// Hue in 360 format
struct int_hsv {
	int h;		// Max: 360
	float s;	// Max: 1.f
	float v;	// Max: 1.f
};

// Hue in 1.f format
struct float_hsv {
	float h;	// Max: 1.f
	float s;	// Max: 1.f
	float v;	// Max: 1.f
};

namespace helpers {
	namespace console {
		void state_to_console(const char* tag, const char* text);
		void state_to_console_color(const char* tag, const char* text);
		void error_to_console(const char* text);
	}

	namespace colors {
		color hsv2color(int_hsv, int alpha = 255);
		color hsv2color(float_hsv, int alpha = 255);
		int_hsv color2hsv(color col);
		float_hsv color2hsv_float(color col);
		color float2color(float* id);
	}
	
	player_t* local_or_spectated();
	bool is_enemy(player_t* player);
};
