#pragma once

struct view_setup_t {
	std::byte pad[172];
	void* csm;
	float fov;
	std::byte pad1[4];
	vec3_t origin;
	vec3_t angles;
	std::byte pad2[4];
	float far_z;
	std::byte pad3[8];
	float aspect_ratio;
};