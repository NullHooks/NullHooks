#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

// I am sorry but I pasted this. I dont know what those vectors mean. If you wanna explain how this eb assist works feel free to open an issue.

void misc::movement::edgebug(c_usercmd* cmd, int old_flags) {
	if (!csgo::local_player) return;
	if (!csgo::local_player->is_alive()) return;

	constexpr float max_radias = M_PI * 2;
	constexpr float step = max_radias / 128;
	constexpr auto thicknesses = std::array{ 23.0f, 21.0f, 3.0f };

	if(input::gobal_input.IsHeld(variables::misc::eb_key) && (csgo::local_player->flags() & fl_onground)) {
		vec3_t pos = csgo::local_player->origin();
		for(auto thickness : thicknesses) {
			for(float a = 0.f; a < max_radias; a += step) {
				vec3_t pt;
				pt.x = (thickness * cos(a)) + pos.x;
				pt.y = (thickness * sin(a)) + pos.y;
				pt.z = pos.z;

				vec3_t pt2 = pt;
				pt2.z -= 6;

				trace_t trace;

				trace_filter flt;
				flt.skip = csgo::local_player;

				ray_t ray;
				ray.initialize(pt, pt2);

				interfaces::trace_ray->trace_ray(ray, MASK_PLAYERSOLID_BRUSHONLY, &flt, &trace);

				if(trace.flFraction != 1.0f && trace.flFraction != 0.0f)
					cmd->buttons |= in_duck;
			}
		}
	}
	/*
	if (input::gobal_input.IsHeld(VK_XBUTTON1) && !(old_flags & fl_onground) && (csgo::local_player->flags() & fl_onground)) {
	cmd->buttons |= in_duck;
	cmd->buttons &= in_jump;
	printf("eb...\n");
	}
	*/
}