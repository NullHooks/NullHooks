#pragma once
#include "source-sdk/math/vector3d.hpp"

class iv_debug_overlay {
public:
	void add_box_overlay(const vec3_t& vecOrigin, const vec3_t& vecAbsMin, const vec3_t& vecAbsMax, const vec3_t& angOrientation, color col, float flDuration) {
		using original_fn = void(__thiscall*)(void*, const vec3_t&, const vec3_t&, const vec3_t&, const vec3_t&, int, int, int, int, float);
		return (*(original_fn**)this)[1](this, vecOrigin, vecAbsMin, vecAbsMax, angOrientation, col.r, col.g, col.b, col.a, flDuration);
	}

	void add_line_overlay(const vec3_t& origin, const vec3_t& dest, color col, bool noDepthTest, float duration) {
		using original_fn = void(__thiscall*)(void*, const vec3_t&, const vec3_t&, int, int, int, bool, float);
		return (*(original_fn**)this)[5](this, origin, dest, col.r, col.g, col.b, noDepthTest, duration);
	}

	bool world_to_screen(const vec3_t& in, vec3_t& out) {
		//int return_value = (*(original_fn**)this)[13](this, in, out);
		//return static_cast<bool>(return_value != 1);
		using original_fn = int(__thiscall*)(iv_debug_overlay*, const vec3_t&, vec3_t&);
		return ((*(original_fn**)this)[13](this, in, out) != 1);
	}
	bool screen_position(const vec3_t& in, vec3_t& out) {
		using original_fn = bool(__thiscall*)(iv_debug_overlay*, const vec3_t&, vec3_t&);
		return (*(original_fn**)this)[11](this, std::ref(in), std::ref(out));
	}
};
