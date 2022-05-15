#pragma once
#include "../../dependencies/utilities/csgo.hpp"

namespace custom_helpers {
	void print_to_console(const char* text);
	void state_to_console(const char* tag, const char* text);
	void state_to_console_color(const char* tag, const char* text);
	color hsv2color(float H, float S, float V);
	color hsv2color_v2(float H, float S, float V);
	color float2color(float* id);
	void draw_bomb_text(float time);

	const float rainbow_speed_f = 0.005f;
	const int rainbow_speed_i = 1;
};
