#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

// https://www.unknowncheats.me/forum/counterstrike-global-offensive/235892-bullet-tracer.html

void visuals::bullet_tracer(i_game_event* event) {
	auto index  = interfaces::engine->get_player_for_user_id(event->get_int("userid"));
	auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(index));
	if (!entity || !entity->is_player()) return;
	if (!csgo::local_player) return;
	const bool is_friend = entity->team() == csgo::local_player->team();
	if (is_friend) {
		if (!variables::misc_visuals::bulletracer_team_target.vector.at(0).state && entity == csgo::local_player)		 return;	// We shot but we dont care
		else if (!variables::misc_visuals::bulletracer_team_target.vector.at(1).state && entity != csgo::local_player) return;	// An ally shot but we dont care
	} else if (!variables::misc_visuals::bulletracer_team_target.vector.at(2).state)									 return;	// An enemy shot but we dont care

	vec3_t hit = { event->get_float("x"), event->get_float("y"), event->get_float("z") };
	auto eyes = entity->get_eye_pos();

	ray_t ray;
	ray.initialize(eyes, hit);

	trace_filter filter;
	filter.skip = entity;
	
	trace_t tr;
	interfaces::trace_ray->trace_ray(ray, MASK_SHOT, &filter, &tr);		// Trace a ray

	constexpr auto time = 5.0f;
	auto line_col = (is_friend) ? variables::colors::friendly_color : variables::colors::enemy_color;					// Use different line color for enemies
	auto box_col = /*(tr.entity && tr.entity->is_player()) ? color(255, 0, 0, 100) :*/ line_col.col.get_custom_alpha(100);		// Removed because hit detection is buggy

	if (variables::misc_visuals::bulletracer_draw_target.vector.at(0).state)											// Draw a line from eyes to the hit point
		interfaces::debug_overlay->add_line_overlay(eyes, hit, line_col, false, time);
	if (variables::misc_visuals::bulletracer_draw_target.vector.at(1).state)											// Draw a box at the hit point
		interfaces::debug_overlay->add_box_overlay(hit, vec3_t(-2, -2, -2), vec3_t(2, 2, 2), vec3_t(0, 0, 0), box_col, time);
}