#pragma once
#include "source-sdk/math/vector3d.hpp"

class iv_debug_overlay {
public:
	virtual void addBoxOverlay(const vec3_t& vecOrigin, const vec3_t& vecAbsMin, const vec3_t& vecAbsMax, const vec3_t& angOrientation, int r, int g, int b, int a, float flDuration) = 0;

	// Used for bullet tracer
	void add_line_overlay(const vec3_t& origin, const vec3_t& dest, int r, int g, int b, bool noDepthTest, float duration) {
		using original_fn = void(__thiscall*)(iv_debug_overlay*, const vec3_t&, const vec3_t&, int, int, int, bool, float);
		return (*(original_fn**)this)[6](this, origin, dest, r, g, b, noDepthTest, duration);
	}

	bool world_to_screen(const vec3_t& in, vec3_t& out) {
		using original_fn = int(__thiscall*)(iv_debug_overlay*, const vec3_t&, vec3_t&);
		int return_value = (*(original_fn * *)this)[13](this, in, out);
		return static_cast<bool>(return_value != 1);
	}
	bool screen_position(const vec3_t& in, vec3_t& out) {
		using original_fn = bool(__thiscall*)(iv_debug_overlay*, const vec3_t&, vec3_t&);
		return (*(original_fn * *)this)[11](this, std::ref(in), std::ref(out));
	}
};