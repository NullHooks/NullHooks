#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

// https://www.unknowncheats.me/forum/counterstrike-global-offensive/235892-bullet-tracer.html

void cbullet_tracer::log(i_game_event* event) {
	//if we receive bullet_impact event
	auto index = interfaces::engine->get_player_for_user_id(event->get_int("userid"));

	if (index != interfaces::engine->get_local_player()) return;

	//get local player
	if (!csgo::local_player) return;

	//get the bullet impact's position
	vec3_t position(event->get_float("x"), event->get_float("y"), event->get_float("z"));

	ray_t ray;
	ray.initialize(csgo::local_player->get_eye_pos(), position);

	//skip local player
	trace_filter filter;
	filter.skip = csgo::local_player;

	//trace a ray
	trace_t tr;
	interfaces::trace_ray->trace_ray(ray, MASK_SHOT, &filter, &tr);

	//use different color when we hit a player
	//auto color = (tr.entity && reinterpret_cast<C_BasePlayer*>(tr.entity)->IsPlayer()) ? color(140, 29, 29, 220) : color(255, 255, 0, 220);

	//push info to our vector
	logs.push_back(cbullet_tracer_info(csgo::local_player->get_eye_pos(), position, interfaces::globals->cur_time, color(255, 0, 0, 220)));
}

void cbullet_tracer::render() {
	if (false) return;	// REPLACE: SETTING BOOL
	if (!csgo::local_player) return;

	//loop through our vector
	for (size_t i = 0; i < logs.size(); i++)
	{
		//get the current item
		auto current = logs.at(i);

		//draw a line from local player's head position to the hit point
		interfaces::debug_overlay->add_line_overlay(current.src, current.dst, current.color.r, current.color.g, current.color.b, true, -1.f);
		//draw a box at the hit point
		interfaces::debug_overlay->add_box_overlay(current.dst, vec3_t(-2, -2, -2), vec3_t(2, 2, 2), vec3_t(0, 0, 0), 255, 0, 0, 127, -1.f);

		//if the item is older than 5 seconds, delete it
		if (fabs(interfaces::globals->cur_time - current.time) > 5.f)
			logs.erase(logs.begin() + i);
	}
}