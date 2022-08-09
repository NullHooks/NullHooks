#pragma once

class collideable_t {
public:
	vec3_t obb_mins() {
		using original_fn = vec3_t*(__thiscall*)(void*);
		return *((*(original_fn**)this)[1](this));
	}
	vec3_t obb_maxs() {
		using original_fn = vec3_t*(__thiscall*)(void*);
		return *((*(original_fn**)this)[2](this));
	}
};