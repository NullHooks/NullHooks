#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

// I am sorry but I pasted this. I dont know what those vectors mean. If you wanna explain how this eb assist works feel free to open an issue.

void misc::movement::edgebug(c_usercmd* cmd, int old_flags) {
    if (!variables::misc::edgebug) return;
    if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game()) return;
    if (!csgo::local_player) return;
    if (!csgo::local_player->is_alive()) return;
    if (!(input::gobal_input.IsHeld(variables::misc::eb_key) && (csgo::local_player->flags() & fl_onground))) return;

    float max_radias = M_PI * 2;
    float step = max_radias / 128;
    float xThick = 23;

    vec3_t pos = csgo::local_player->abs_origin();
    for (float a = 0.f; a < max_radias; a += step) {
        vec3_t pt;
        pt.x = (xThick * cos(a)) + pos.x;
        pt.y = (xThick * sin(a)) + pos.y;
        pt.z = pos.z;

        vec3_t pt2 = pt;
        pt2.z -= 6;

        trace_t trace;
            
        trace_filter flt;
        flt.skip = csgo::local_player;

        ray_t ray;
        ray.initialize(pt, pt2);

        interfaces::trace_ray->trace_ray(ray, 0x1400B, &flt, &trace);

        if (trace.flFraction != 1.0f && trace.flFraction != 0.0f)
            cmd->buttons |= in_duck;
    }

    for (float a = 0.f; a < max_radias; a += step) {
        vec3_t pt;
        pt.x = ((xThick - 2.f) * cos(a)) + pos.x;
        pt.y = ((xThick - 2.f) * sin(a)) + pos.y;
        pt.z = pos.z;

        vec3_t pt2 = pt;
        pt2.z -= 6;

        trace_t trace;

        trace_filter flt;
        flt.skip = csgo::local_player;

        ray_t ray;
        ray.initialize(pt, pt2);

        interfaces::trace_ray->trace_ray(ray, 0x1400B, &flt, &trace);

        if (trace.flFraction != 1.f && trace.flFraction != 0.f)
            cmd->buttons |= in_duck;
    }

    for (float a = 0.f; a < max_radias; a += step)
    {
        vec3_t pt;
        pt.x = ((xThick - 20.f) * cos(a)) + pos.x;
        pt.y = ((xThick - 20.f) * sin(a)) + pos.y;
        pt.z = pos.z;

        vec3_t pt2 = pt;
        pt2.z -= 6;

        trace_t trace;

        trace_filter flt;
        flt.skip = csgo::local_player;

        ray_t ray;
        ray.initialize(pt, pt2);

        interfaces::trace_ray->trace_ray(ray, 0x1400B, &flt, &trace);

        if (trace.flFraction != 1.f && trace.flFraction != 0.f)
        {
            cmd->buttons |= in_duck;
        }
    }
}