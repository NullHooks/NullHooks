#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

bool should_duck = false;
int old_flags = 0;
float height_diff;

void misc::movement::pre_pred_jumpbug(c_usercmd* cmd) {
    if (false) return;
    if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game()) return;
    if (!csgo::local_player) return;
    if (csgo::local_player->move_type() == movetype_ladder || csgo::local_player->move_type() == movetype_noclip) return;

    old_flags = csgo::local_player->flags();
    if (should_duck && !(old_flags & fl_onground))
        cmd->buttons |= in_duck;

    vec3_t origin = csgo::local_player->origin();

    ray_t ray;
    ray.initialize(origin, vec3_t(origin.x, origin.y, origin.z - 8192.0f));

    trace_filter filter;
    filter.skip = csgo::local_player;

    trace_t trace;
    interfaces::trace_ray->trace_ray(ray, MASK_SHOT, &filter, &trace);

    height_diff = origin.z - trace.end.z;
    if (height_diff > 200.0f)
        should_duck = true;
}

void misc::movement::post_pred_jumpbug(c_usercmd* cmd) {
    if (false) return;
    if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game()) return;
    if (!csgo::local_player) return;
    if (csgo::local_player->move_type() == movetype_ladder || csgo::local_player->move_type() == movetype_noclip) return;

    if (!(old_flags & fl_onground) && (csgo::local_player->flags() & fl_onground) && should_duck) {
        cmd->buttons &= ~in_duck;
        //cmd->buttons &= in_jump;
        should_duck = false;
    }
}