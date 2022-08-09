#pragma once

#pragma region STRUCTS
// Hue in 360 format
struct int_hsv {
	int h;		// Max: 360
	float s;	// Max: 1.f
	float v;	// Max: 1.f

	int_hsv() = default;
	int_hsv(int_hsv& hsv) {
		this->h = hsv.h;
		this->s = hsv.s;
		this->v = hsv.v;
	}

	int_hsv(int h, float s, float v) {
		this->h = h;
		this->s = s;
		this->v = v;
	}
};

// Hue in 1.f format
struct float_hsv {
	float h, s, v;	// Max: 1.f

	float_hsv() = default;
	float_hsv(float_hsv& hsv) {
		this->h = hsv.h;
		this->s = hsv.s;
		this->v = hsv.v;
	}

	float_hsv(float h, float s, float v) {
		this->h = h;
		this->s = s;
		this->v = v;
	}

	// For comparing colors
	bool operator != (float_hsv other) {
		return (other.h == h && other.s == s && other.v == v);
	}
};

struct float_color {
	float r, g, b;	// Max: 1.f

	float_color() = default;
	float_color(float_color& hsv) {
		this->r = hsv.r;
		this->g = hsv.g;
		this->b = hsv.b;
	}

	float_color(float r, float g, float b) {
		this->r = r;
		this->g = g;
		this->b = b;
	}

	float_color(color col) {
		this->r = (float)col.r / 255.f;
		this->g = (float)col.g / 255.f;
		this->b = (float)col.b / 255.f;
	}
};
#pragma endregion

#pragma region ENUMS
enum chat_colors {
	CHAT_COLOR_WHITE = 0x01,
	CHAT_COLOR_RED,
	CHAT_COLOR_LIGHT_PURPLE,
	CHAT_COLOR_GREEN,
	CHAT_COLOR_LIGHT_GREEN,
	CHAT_COLOR_LIME,
	CHAT_COLOR_LIGHT_RED,
	CHAT_COLOR_GRAY,
	CHAT_COLOR_LIGHT_YELLOW,
	CHAT_COLOR_GRAY_BLUE,
	CHAT_COLOR_LIGHT_BLUE,
	CHAT_COLOR_BLUE,
	CHAT_COLOR_PURPLE,
};
#pragma endregion

namespace helpers {
	namespace console {
		void state_to_console(const char* tag, const char* text);
		void state_to_console_color(const char* tag, const char* text);
		void error_to_console(const char* text);
	}

	namespace colors {
		color hsv2color(int_hsv, int alpha = 255);
		color hsv_float2color(float_hsv, int alpha = 255);
		int_hsv color2hsv(color col);
		float_hsv color2hsv_float(color col);
		color float2color(float* id);
	}

	namespace chat {
		void print(std::string str);
		void print(std::string str, char col);		// Prints 'NullHooks | str' with color
		void load_config(std::string config_name);
		void save_config(std::string config_name);
	}

	template <typename T>
	static constexpr auto relative_to_absolute(uint8_t* address);

	player_t* local_or_spectated();
	bool is_enemy(player_t* player);
	std::string strip(std::string str);
};
