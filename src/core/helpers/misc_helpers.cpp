#include "misc_helpers.hpp"

void custom_helpers::print_to_console(const char* text) {
	if (text == nullptr) return;
	interfaces::console->console_printf(text);
}

void custom_helpers::state_to_console(const char* tag,  const char* text) {
	if (text == nullptr || tag == nullptr) return;
	interfaces::console->console_printf("[NullHooks] [%s] %s\n", tag, text);
}

void custom_helpers::state_to_console_color(const char* tag, const char* text) {
	if (text == nullptr || tag == nullptr) return;
	
	// ???
	interfaces::console->console_color_printf(color::red(255), "[NullHooks] [%s] %s\n", tag, text);
	interfaces::console->console_color_printf(color::blue(255), "[NullHooks] [%s] %s\n", tag, text);
	interfaces::console->console_color_printf(color::green(255), "[NullHooks] [%s] %s\n", tag, text);
}

/* hsv2color(int_hsv): Returns color from hsv. Hue in 360 format. */
color custom_helpers::hsv2color(int_hsv hsv) {
	float fC = hsv.v * hsv.s;							// Chroma
	float fHPrime = fmod(hsv.h / 60.0, 6);
	float fX = fC * (1 - fabs(fmod(fHPrime, 2) - 1));
	float fM = hsv.v - fC;

	if (0 <= fHPrime && fHPrime < 1)
		return color(fC*255, fX*255, 0, 255);
	else if (1 <= fHPrime && fHPrime < 2)
		return color(fX*255, fC*255, 0, 255);
	else if (2 <= fHPrime && fHPrime < 3)
		return color(0, fC*255, fX*255, 255);
	else if (3 <= fHPrime && fHPrime < 4)
		return color(0, fX*255, fC*255, 255);
	else if (4 <= fHPrime && fHPrime < 5)
		return color(fX*255, 0, fC*255, 255);
	else if (5 <= fHPrime && fHPrime < 6)
		return color(fC*255, 0, fX*255, 255);
	else
		return color(0, 0, 0);
}

/* hsv2color(float_hsv): Returns color from hsv. Hue in 1.f format. */
color custom_helpers::hsv2color(float_hsv hsv) {
	int_hsv converted = {
		hsv.h * 360.f,		// So its in 1.f format
		hsv.s,
		hsv.v
	};

	return hsv2color(converted);
}

/*
 * See: https://gist.github.com/r4v10l1/5f559419bb1f27eb22ea5b9da0343b1b
 * Returns hsv from color. Hue in 360 format.
 */
int_hsv custom_helpers::color2hsv(color col) {
	int_hsv result;

	float fCMax = max(max(col.r, col.g), col.b);
	float fCMin = min(min(col.r, col.g), col.b);
	float fDelta = fCMax - fCMin;

	if (fDelta > 0) {
		if (fCMax == col.r)
			result.h = 60 * (fmod(((col.g - col.b) / fDelta), 6));
		else if (fCMax == col.g)
			result.h = 60 * (((col.b - col.r) / fDelta) + 2);
		else if (fCMax == col.b)
			result.h = 60 * (((col.r - col.g) / fDelta) + 4);

		if (fCMax > 0) result.s = fDelta / fCMax;
		else result.s = 0;

		result.v = fCMax;
	} else {
		result.h = 0;
		result.s = 0;
		result.v = fCMax;
	}

	if (result.h < 0) result.h = 360 + result.h;	// Revert if too small
	
	return result;
}

/* Returns hsv from color. Hue in 1.f format. */
float_hsv custom_helpers::color2hsv_float(color col) {
	int_hsv int_result = color2hsv(col);
	float_hsv result = { 
		int_result.h / 360.f,		// So its in 1.f format
		int_result.s,
		int_result.v
	};

	return result;
}

color custom_helpers::float2color(float* id) {
	if (*id < 1.f)				return color(255, 0, 0);
	else if (*id < 2.f)			return color(255, 128, 0);
	else if (*id < 3.f)			return color(255, 255, 0);
	else if (*id < 4.f)			return color(128, 255, 0);
	else if (*id < 5.f)			return color(0, 255, 0);
	else if (*id < 6.f)			return color(0, 255, 128);
	else if (*id < 7.f)			return color(0, 255, 255);
	else if (*id < 8.f)			return color(0, 128, 255);
	else if (*id < 9.f)			return color(0, 0, 255);
	else if (*id < 10.f)		return color(128, 0, 255);
	else if (*id < 11.f)		return color(255, 0, 255);
	else if (*id < 12.f)		return color(255, 0, 128);
	else						*id = 0.f;

	return color(255, 0, 0);
}

void custom_helpers::draw_bomb_text(float time) {
	char exp_time[64];
	sprintf_s(exp_time, "%.2f", time);

	const std::string exp_time_str_base = "Bomb will explode in: ";
	const std::string exp_time_str = std::string(exp_time);
	const std::string total = exp_time_str_base + exp_time_str;
	const std::wstring c_exp_time_str_base = std::wstring(exp_time_str_base.begin(), exp_time_str_base.end());
	const std::wstring c_exp_time_str = std::wstring(exp_time_str.begin(), exp_time_str.end());
	const std::wstring c_total = std::wstring(total.begin(), total.end());

	const color base_color = color(255, 130, 0, 255);
	const color bomb_color_text_color = (time > 10.f) ? color(255, 190, 0, 255) : color::red(255);

	int screen_width, screen_height;
	interfaces::engine->get_screen_size(screen_width, screen_height);

	int width, height;
	interfaces::surface->draw_text_font(render::fonts::watermark_font);
	interfaces::surface->get_text_size(render::fonts::watermark_font, c_total.c_str(), width, height);
	const int x_pos = screen_width / 2 - width / 2;
	const int y_pos = 95;

	interfaces::surface->draw_text_pos(x_pos, y_pos);

	interfaces::surface->set_text_color(base_color.r, base_color.g, base_color.b, base_color.a);
	interfaces::surface->draw_render_text(c_exp_time_str_base.c_str(), wcslen(c_exp_time_str_base.c_str()));

	interfaces::surface->set_text_color(bomb_color_text_color.r, bomb_color_text_color.g, bomb_color_text_color.b, bomb_color_text_color.a);
	interfaces::surface->draw_render_text(c_exp_time_str.c_str(), wcslen(c_exp_time_str.c_str()));
}