#pragma once
#include "../../dependencies/utilities/csgo.hpp"

namespace custom_helpers {
	void print_to_console(const char* text);
	void state_to_console(const char* tag, const char* text);
	void state_to_console_color(const char* tag, const char* text);
	color hsv2color(float H, float S, float V);
	color hsv_float2color(float hue, float saturation, float brightness);

	const float rainbow_speed = 0.001f;
};
