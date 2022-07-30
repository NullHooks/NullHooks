#include "dependencies/utilities/csgo.hpp"
#include "core/helpers/helpers.hpp"

#pragma region CONSOLE
void helpers::console::state_to_console(const char* tag,  const char* text) {
	interfaces::console->printf("[NullHooks] [%s] %s\n", tag, text);
}

void helpers::console::state_to_console_color(const char* tag, const char* text) {
	interfaces::console->color_printf(valve_color_t{ 255, 255, 255, 255 }, "[");
	interfaces::console->color_printf(valve_color_t{ 200,   0,   0, 255 }, "NullHooks");
	interfaces::console->color_printf(valve_color_t{ 255, 255, 255, 255 }, "] [");
	interfaces::console->color_printf(valve_color_t{   0, 165, 230, 255 }, tag);
	interfaces::console->color_printf(valve_color_t{ 255, 255, 255, 255 }, "] %s\n", text);
}

void helpers::console::error_to_console(const char* text) {
	interfaces::console->color_printf(valve_color_t{ 255, 255, 255, 255 }, "[");
	interfaces::console->color_printf(valve_color_t{ 200,   0,   0, 255 }, "NullHooks");
	interfaces::console->color_printf(valve_color_t{ 255, 255, 255, 255 }, "] [");
	interfaces::console->color_printf(valve_color_t{ 230, 130,  50, 255 }, "Error");
	interfaces::console->color_printf(valve_color_t{ 255, 255, 255, 255 }, "] %s\n", text);
}
#pragma endregion

#pragma region COLORS
/* hsv2color(int_hsv): Returns color from hsv. Hue in 360 format. */
color helpers::colors::hsv2color(int_hsv hsv, int alpha) {
	float fC = hsv.v * hsv.s;							// Chroma
	float fHPrime = fmod(hsv.h / 60.0, 6);
	float fX = fC * (1 - fabs(fmod(fHPrime, 2) - 1));
	float fM = hsv.v - fC;

	color ret = color(0, 0, 0, alpha);
	float_color col = { 0.f, 0.f, 0.f };

	if (0 <= fHPrime && fHPrime < 1)			col = float_color{ fC,  fX,  0.f };
	else if (1 <= fHPrime && fHPrime < 2)		col = float_color{ fX,  fC,  0.f };
	else if (2 <= fHPrime && fHPrime < 3)		col = float_color{ 0.f, fC,  fX  };
	else if (3 <= fHPrime && fHPrime < 4)		col = float_color{ 0.f, fX,  fC  };
	else if (4 <= fHPrime && fHPrime < 5)		col = float_color{ fX,  0.f, fC  };
	else if (5 <= fHPrime && fHPrime < 6)		col = float_color{ fC,  0.f, fX  };
	else										col = float_color{ 0.f, 0.f, 0.f };

	col.r += fM;
	col.g += fM;
	col.b += fM;

	ret.r = col.r * 255;
	ret.g = col.g * 255;
	ret.b = col.b * 255;
	ret.a = alpha;

	return ret;
}

/* hsv2color(float_hsv): Returns color from hsv. Hue in 1.f format. */
color helpers::colors::hsv_float2color(float_hsv hsv, int alpha) {
	int_hsv converted(
		hsv.h * 360.f,		// So its in 1.f format
		hsv.s,
		hsv.v
	);

	return hsv2color(converted, alpha);
}

/*
 * See: https://gist.github.com/r4v10l1/5f559419bb1f27eb22ea5b9da0343b1b
 * Returns hsv from color. Hue in 360 format.
 */
int_hsv helpers::colors::color2hsv(color col) {
	int_hsv result;
	float_color f_col{ col.r / 255.f, col.g / 255.f, col.b / 255.f };		// Convert 255 rgb to 1.f rgb

	float fCMax = max(max(f_col.r, f_col.g), f_col.b);
	float fCMin = min(min(f_col.r, f_col.g), f_col.b);
	float fDelta = fCMax - fCMin;

	if (fDelta > 0) {
		if (fCMax == f_col.r)
			result.h = 60.f * (fmod(((f_col.g - f_col.b) / fDelta), 6));
		else if (fCMax == f_col.g)
			result.h = 60.f * (((f_col.b - f_col.r) / fDelta) + 2);
		else if (fCMax == f_col.b)
			result.h = 60.f * (((f_col.r - f_col.g) / fDelta) + 4);

		if (fCMax > 0.f) result.s = fDelta / fCMax;
		else result.s = 0.f;

		result.v = fCMax;
	} else {
		result.h = 0.f;
		result.s = 0.f;
		result.v = fCMax;
	}

	if (result.h < 0.f) result.h = 360.f + result.h;	// Revert if too small
	
	return result;
}

/* Returns hsv from color. Hue in 1.f format. */
float_hsv helpers::colors::color2hsv_float(color col) {
	int_hsv int_result = color2hsv(col);
	float_hsv result = { 
		(float)int_result.h / 360.f,		// So its in 1.f format
		int_result.s,
		int_result.v
	};

	return result;
}

color helpers::colors::float2color(float* id) {
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
#pragma endregion

#pragma region MISC
template <typename T>
static constexpr auto relativeToAbsolute(uint8_t* address) noexcept {
	return (T)(address + 4 + *reinterpret_cast<std::int32_t*>(address));
}

// Get localplayer or the player we are spectating
player_t* helpers::local_or_spectated() {
	if (!csgo::local_player) return nullptr;
	return (csgo::local_player->is_alive()) ? csgo::local_player : reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity_handle(csgo::local_player->observer_target()));
}

// Checks if its enemy from localplayer
bool helpers::is_enemy(player_t* player) {
	if (!csgo::local_player || !player) return false;

	using fn = bool(__thiscall*)(player_t*, player_t*);
	static fn isOtherEnemy = relativeToAbsolute<fn>(utilities::pattern_scan("client.dll", "8B CE E8 ? ? ? ? 02 C0") + 3);

	isOtherEnemy(csgo::local_player, player);
}
#pragma endregion
