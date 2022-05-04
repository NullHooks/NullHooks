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


color custom_helpers::hsv2color(float H, float S, float V) {
    color col;
    float s = S / 100;
    float v = V / 100;
    float C = s * v;
    float X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
    float m = v - C;
    float r, g, b;
    if (H >= 0 && H < 60) {
        r = C, g = X, b = 0;
    } else if (H >= 60 && H < 120) {
        r = X, g = C, b = 0;
    } else if (H >= 120 && H < 180) {
        r = 0, g = C, b = X;
    } else if (H >= 180 && H < 240) {
        r = 0, g = X, b = C;
    } else if (H >= 240 && H < 300) {
        r = X, g = 0, b = C;
    } else {
        r = C, g = 0, b = X;
    }
    col.r = (r + m) * 255;
    col.g = (g + m) * 255;
    col.b = (b + m) * 255;
    col.a = 255;
    return col;
}

color custom_helpers::hsv_float2color(float hue, float saturation, float brightness) {
	float h = hue == 1.0f ? 0 : hue * 6.0f;
	float f = h - (int)h;
	float p = brightness * (1.0f - saturation);
	float q = brightness * (1.0f - saturation * f);
	float t = brightness * (1.0f - (saturation * (1.0f - f)));

	if (h < 1)
	{
		return color(
			(unsigned char)(brightness * 255),
			(unsigned char)(t * 255),
			(unsigned char)(p * 255)
		);
	}
	else if (h < 2)
	{
		return color(
			(unsigned char)(q * 255),
			(unsigned char)(brightness * 255),
			(unsigned char)(p * 255)
		);
	}
	else if (h < 3)
	{
		return color(
			(unsigned char)(p * 255),
			(unsigned char)(brightness * 255),
			(unsigned char)(t * 255)
		);
	}
	else if (h < 4)
	{
		return color(
			(unsigned char)(p * 255),
			(unsigned char)(q * 255),
			(unsigned char)(brightness * 255)
		);
	}
	else if (h < 5)
	{
		return color(
			(unsigned char)(t * 255),
			(unsigned char)(p * 255),
			(unsigned char)(brightness * 255)
		);
	}
	else
	{
		return color(
			(unsigned char)(brightness * 255),
			(unsigned char)(p * 255),
			(unsigned char)(q * 255)
		);
	}
}