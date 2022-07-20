#pragma once
#include "dependencies/utilities/csgo.hpp"

// Hue in 360 format
typedef struct IHSV {
	int h;		// Max: 360
	float s;	// Max: 1.f
	float v;	// Max: 1.f
} int_hsv;

// Hue in 1.f format
typedef struct FHSV {
	float h;	// Max: 1.f
	float s;	// Max: 1.f
	float v;	// Max: 1.f
} float_hsv;

namespace custom_helpers {
	void state_to_console(const char* tag, const char* text);
	void state_to_console_color(const char* tag, const char* text);
	color hsv2color(int_hsv, int alpha = 255);
	color hsv2color(float_hsv, int alpha = 255);
	int_hsv color2hsv(color col);
	float_hsv color2hsv_float(color col);
	color float2color(float* id);
	void draw_bomb_text(float time);

	player_t* local_or_spectated();

	const float rainbow_speed_f = 0.005f;
	const int rainbow_speed_i = 1;
};
