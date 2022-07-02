#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

// https://www.unknowncheats.me/forum/counterstrike-global-offensive/235892-bullet-tracer.html

void visuals::bullet_tracer(i_game_event* event) {
	//if we receive bullet_impact event
	auto index  = interfaces::engine->get_player_for_user_id(event->get_int("userid"));
	auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(index));

	if (!entity || !entity->is_player()) return;
	if (!csgo::local_player) return;

	//get the bullet impact's position
	vec3_t pos = { event->get_float("x"), event->get_float("y"), event->get_float("z") };

	auto eyes = entity->get_eye_pos();

	ray_t ray;
	ray.initialize(eyes, pos);

	trace_filter filter;
	filter.skip = entity;

	//trace a ray
	trace_t tr;
	interfaces::trace_ray->trace_ray(ray, MASK_SHOT, &filter, &tr);

	constexpr auto time = 5.0f;

	//use different line color for enemies
	auto line_col = (entity->team() == csgo::local_player->team()) ? color(0, 255, 0) : color(0, 0, 255);

	//use different box color when we hit a player
	auto box_col = (tr.entity && tr.entity->is_player()) ? color(255, 0, 0, 100) : color(255, 255, 0, 100);

	//draw a line from eyes to the hit point
	interfaces::debug_overlay->add_line_overlay(eyes, pos, line_col, false, time);
	//draw a box at the hit point
	interfaces::debug_overlay->add_box_overlay(pos, vec3_t(-2, -2, -2), vec3_t(2, 2, 2), vec3_t(0, 0, 0), box_col, time);

}