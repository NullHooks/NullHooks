#include "../features.hpp"

void aim::run_aimbot(c_usercmd* cmd) {
	if (!(cmd->buttons & cmd_buttons::in_attack)) return;		// User not attacking
	if (!variables::aim::aimbot) return;
	if (csgo::local_player->is_defusing()) return;

	weapon_t* active_weapon = csgo::local_player->active_weapon();
	if (!active_weapon) return;

	const int weapon_type = active_weapon->get_weapon_data()->weapon_type;
	switch (weapon_type) {										// Only aimbot on weapons that shoot
		case WEAPONTYPE_MACHINEGUN:
		case WEAPONTYPE_RIFLE:
		case WEAPONTYPE_SHOTGUN:
		case WEAPONTYPE_SNIPER_RIFLE:
		case WEAPONTYPE_PISTOL: {
			if (!active_weapon->clip1_count()) return;			// No ammo so don't aimbot

			if (weapon_type == WEAPONTYPE_SNIPER_RIFLE
				&& csgo::local_player->is_scoped()
				&& !variables::aim::aimbot_noscope) return;		// We are not scoped and have the noscope option disabled

			break;
		}
		default: return;
	}

	// We reached here without return so we are good

	vec3_t best_angle{};
	float best_fov{ variables::aim::aimbot_fov };	// This variable will store the fov of the closest player to crosshair, we start it as the fov setting

	for (int n = 1; n <= interfaces::globals->max_clients; n++) {
		auto cur_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(n));
		if (!cur_player
			|| cur_player->dormant()
			|| cur_player->is_alive()
			|| cur_player->has_gun_game_immunity()
			|| cur_player->team() == csgo::local_player->team()) continue;

		matrix_t bones[128];
		if (!cur_player->setup_bones(bones, 128, 256, interfaces::globals->cur_time)) continue;

		vec3_t local_aim_punch{};	// Initialize at 0 because we only want aim punch with rifles
		switch (weapon_type) {
			case WEAPONTYPE_RIFLE:
			case WEAPONTYPE_SUBMACHINEGUN:
			case WEAPONTYPE_MACHINEGUN:
				local_aim_punch = csgo::local_player->aim_punch_angle();
		}

		vec3_t local_eye_pos = csgo::local_player->get_eye_pos();
		vec3_t cur_player_head = bones[8].get_origin();

		ray_t ray;
		ray.initialize(local_eye_pos, cur_player_head);	// We need to initialize the ray like this

		trace_filter filter(csgo::local_player);

		trace_t* trace;
		interfaces::trace_ray->trace_ray(ray, MASK_SHOT, &filter, trace);

		if (!trace->entity || trace->flFraction < 0.97) return;

		vec3_t enemy_angle{ (cur_player_head - local_eye_pos).to_angle() - (cmd->viewangles + local_aim_punch) };

		const float fov = std::hypot(enemy_angle.x, enemy_angle.y);
		if (fov < best_fov) {	// First time checks the fov setting, then will overwrite if it finds a player that is closer to crosshair
			best_fov = fov;
			best_angle = enemy_angle;
		}
	}

	cmd->viewangles += best_angle * variables::aim::aimbot_smoothing;
}