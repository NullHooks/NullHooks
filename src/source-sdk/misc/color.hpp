#pragma once
#include <cstdint>
#include <d3d9.h>

/*
 * Thank you designer :
 *   https://www.unknowncheats.me/forum/counterstrike-global-offensive/385547-printing-console.html
 */
struct valve_color_t {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

struct color {
	int a, r, g, b;
	color() = default;
	color(const int r, const int g, const int b, const int a = 255) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	explicit color(const uint32_t color) {
		this->a = (color >> 24) & 0xff;
		this->r = (color >> 16) & 0xff;
		this->g = (color >> 8) & 0xff;
		this->b = (color & 0xff);
	}

	static color from_uint(const uint32_t uint) {
		return color(uint);
	}

	// Returns the same color but with custom alpha. Used to do the color picker alpha fade.
	color get_custom_alpha(const int alpha) {
		return color{ this->r, this->g, this->b, alpha };
	}

	static D3DCOLOR from_color(const color col) {
		return D3DCOLOR_ARGB(col.a, col.r, col.g, col.b);
	}

	static color interpolate(const color& first_color, const color& second_color, const float multiplier) {
		return color(first_color.r + std::clamp(multiplier, 0.f, 1.f) * (second_color.r - first_color.r),
			first_color.g + std::clamp(multiplier, 0.f, 1.f) * (second_color.g - first_color.g),
			first_color.b + std::clamp(multiplier, 0.f, 1.f) * (second_color.b - first_color.b),
			first_color.a + std::clamp(multiplier, 0.f, 1.f) * (second_color.a - first_color.a));
	}

	// For comparing colors
	bool operator != (color other) {
		return (other.r == r && other.g == g && other.b == b && other.a == a);
	}

	auto operator/(float other) {
		color ret;
		ret.r = r / other;
		ret.g = g / other;
		ret.b = b / other;
		return ret;
	}

	static color black(const int a = 255) { return { 0, 0, 0, a }; }
	static color white(const int a = 255) { return { 255, 255, 255, a }; }
	static color red(const int   a = 255) { return { 255, 0, 0, a }; }
	static color green(const int a = 255) { return { 0, 255, 0, a }; }
	static color blue(const int  a = 255) { return { 0, 0, 255, a }; }
};
