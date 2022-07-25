#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

void aim::triggerbot(c_usercmd* cmd) {
	if (!variables::aim::triggerbot) return;
	if (!input::gobal_input.IsHeld(variables::aim::triggerbot_key)) return;
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game()) return;
	if (!csgo::local_player) return;
	if (!aimbot_weapon_check(true)) return;

	vec3_t eye_pos = csgo::local_player->get_eye_pos();									// Start
	vec3_t ang = cmd->viewangles + csgo::local_player->aim_punch_angle() * 2.f;
	const vec3_t dst = eye_pos + math::angle_vector(ang) * 2500.f;	// End

	ray_t ray;
	ray.initialize(eye_pos, dst);

	trace_filter filter(csgo::local_player);

	trace_t trace;
	interfaces::trace_ray->trace_ray(ray, 0x46004009, &filter, &trace );
	if (!trace.entity 
		|| !trace.entity->is_player()
		|| !trace.entity->is_alive()
		|| trace.entity->dormant()
		|| trace.entity->has_gun_game_immunity()) return;

	if (!trace.entity->is_alive() || (!variables::aim::target_friends && !helpers::is_enemy(trace.entity))) return;

	static int cur_delay = 0;			// ms
	if (cur_delay >= (int)variables::aim::triggerbot_delay) {
		cur_delay = 0;	// Reset and shoot
		cmd->buttons |= in_attack;		// Shoot
	}
	cur_delay++;
}