#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

bool aim::can_fire(player_t* target) {
	weapon_t* active_weapon = target->active_weapon();
	if (!active_weapon) return false;

	if (active_weapon->is_non_aim()) return false;			// Bad weapon or no ammo so don't aimbot
	if (target->next_attack() > interfaces::globals->cur_time) return false;
	if (active_weapon->next_primary_attack() > interfaces::globals->cur_time) return false;

	return true;
}

// Checks if the current weapon can shoot and all that
bool aim::aimbot_weapon_check(bool check_scope) {
	if (csgo::local_player->is_defusing()) return false;

	weapon_t* active_weapon = csgo::local_player->active_weapon();
	if (!active_weapon) return false;

	const auto weapon_data = active_weapon->get_weapon_data();
	if (!weapon_data) return false;

	switch (weapon_data->weapon_type) {									// Only aimbot on weapons that shoot
		case WEAPONTYPE_MACHINEGUN:
		case WEAPONTYPE_RIFLE:
		case WEAPONTYPE_SUBMACHINEGUN:
		case WEAPONTYPE_SHOTGUN:
		case WEAPONTYPE_SNIPER_RIFLE:
		case WEAPONTYPE_PISTOL: {
			if (!can_fire(csgo::local_player)) return false;					// Check if we can fire

			if (check_scope) {
				if (weapon_data->weapon_type == WEAPONTYPE_SNIPER_RIFLE
					&& !csgo::local_player->is_scoped()
					&& !variables::aim::aimbot_noscope) return false;		// We are not scoped and have the noscope option disabled
			}

			break;
		}
		default: return false;
	}

	// (We reached here without return so we are good to use aimbot)
	return true;
}

vec3_t get_best_target(c_usercmd* cmd, weapon_t* active_weapon) {
	// Store selected hitboxes
	std::vector<int> all_hitboxes = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18 };	// For bodyaim if lethal
	std::vector<int> selected_hitboxes;
	if (variables::aim::hitboxes.vector[0].state) {		// Head
		selected_hitboxes.emplace_back(hitbox_head);
	}
	if (variables::aim::hitboxes.vector[1].state) {		// Neck
		selected_hitboxes.emplace_back(hitbox_neck);
	}
	if (variables::aim::hitboxes.vector[2].state) {		// Chest
		selected_hitboxes.emplace_back(hitbox_pelvis);
		selected_hitboxes.emplace_back(hitbox_stomach);
		selected_hitboxes.emplace_back(hitbox_lower_chest);
		selected_hitboxes.emplace_back(hitbox_chest);
		selected_hitboxes.emplace_back(hitbox_upper_chest);
	}
	if (variables::aim::hitboxes.vector[3].state) {		// Arms
		selected_hitboxes.emplace_back(hitbox_right_hand);
		selected_hitboxes.emplace_back(hitbox_left_hand);
		selected_hitboxes.emplace_back(hitbox_right_upper_arm);
		selected_hitboxes.emplace_back(hitbox_left_upper_arm);
		selected_hitboxes.emplace_back(hitbox_right_forearm);
		selected_hitboxes.emplace_back(hitbox_left_forearm);

	}
	if (variables::aim::hitboxes.vector[4].state) {		// Legs
		selected_hitboxes.emplace_back(hitbox_right_thigh);
		selected_hitboxes.emplace_back(hitbox_left_thigh);
		selected_hitboxes.emplace_back(hitbox_right_calf);
		selected_hitboxes.emplace_back(hitbox_left_calf);
		selected_hitboxes.emplace_back(hitbox_right_foot);
		selected_hitboxes.emplace_back(hitbox_left_foot);

	}

	float best_fov = variables::aim::aimbot_fov;	// This variable will store the fov of the closest player to crosshair, we start it as the fov setting
	vec3_t best_target(0, 0, 0);					// Position of best hitbox. Will be returned
	const auto weapon_data = active_weapon->get_weapon_data();
	if (!weapon_data) return best_target;

	// Check each player
	for (int n = 1; n <= interfaces::globals->max_clients; n++) {
		auto cur_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(n));
		if (!cur_player
			|| cur_player == csgo::local_player
			|| !cur_player->is_alive()
			|| cur_player->dormant()
			|| cur_player->has_gun_game_immunity()) continue;
		if (cur_player->team() == csgo::local_player->team() && !variables::aim::target_friends) continue;

		auto local_eye_pos = csgo::local_player->get_eye_pos();		// Get eye pos from origin player_t

		for (const auto hitbox : all_hitboxes) {
			auto hitbox_pos = cur_player->get_hitbox_position(hitbox);
			bool enabled_hitbox = std::find(selected_hitboxes.begin(), selected_hitboxes.end(), hitbox) != selected_hitboxes.end();

			// Ignore everything if we have "ignore walls" setting (2)
			if (variables::aim::autowall.idx != 2) {
				if (!aim::autowall::handle_walls(csgo::local_player, cur_player, hitbox_pos, weapon_data, (int)variables::aim::min_damage, enabled_hitbox))
					continue;
			} else if (!enabled_hitbox) {
				continue;	// We are trying to use ignore walls with disabled hitbox
			}

			vec3_t aim_angle = math::calculate_relative_angle(local_eye_pos, hitbox_pos, cmd->viewangles);
			aim_angle.clamp();

			// First time checks the fov setting, then will overwrite if it finds a player that is closer to crosshair
			const float fov = std::hypot(aim_angle.x, aim_angle.y);
			if (fov < best_fov) {
				best_fov = fov;
				best_target = hitbox_pos;
			}
		}
	}

	return best_target;		// vec3_t position of the best bone
}

void aim::run_aimbot(c_usercmd* cmd) {
	if (!(variables::aim::autofire && input::gobal_input.IsHeld(variables::aim::aimbot_key.key))	// Not holding aimbot key
		&& !(!variables::aim::autofire && (cmd->buttons & cmd_buttons::in_attack))) return;			// or not attacking
	if (!variables::aim::aimbot) return;
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game()) return;
	if (!csgo::local_player) return;
	if (!aimbot_weapon_check(true)) return;

	// We need to get weapon_type here too for aim_punch and for autowall
	weapon_t* active_weapon = csgo::local_player->active_weapon();
	if (!active_weapon) return;
	const auto weapon_data = active_weapon->get_weapon_data();
	if (!weapon_data) return;

	auto local_eye_pos = csgo::local_player->get_eye_pos();		// Get eye pos from origin player_t
	vec3_t target = get_best_target(cmd, active_weapon);
	if (target.is_zero()) return;

	vec3_t aim_angle = math::calculate_angle(local_eye_pos, target);
	aim_angle.clamp();

	vec3_t local_aim_punch{};	// Initialize at 0 because we only want aim punch with rifles
	if (variables::aim::non_rifle_aimpunch) {
		local_aim_punch = csgo::local_player->aim_punch_angle();
	} else {
		switch (weapon_data->weapon_type) {
			case WEAPONTYPE_RIFLE:
			case WEAPONTYPE_SUBMACHINEGUN:
			case WEAPONTYPE_MACHINEGUN:
				local_aim_punch = csgo::local_player->aim_punch_angle();
		}
	}

	static auto recoil_scale = interfaces::console->get_convar("weapon_recoil_scale")->get_float();
	vec3_t enemy_angle = (aim_angle - (local_aim_punch * recoil_scale)) - cmd->viewangles;
	enemy_angle.clamp();

	vec3_t final_angle = enemy_angle;	
	if (!variables::aim::silent) final_angle *= (1.f - variables::aim::aimbot_smoothing);	// Scale acording to smoothing if not silent
	
	cmd->viewangles += final_angle;
	if (variables::aim::autofire && input::gobal_input.IsHeld(variables::aim::aimbot_key.key))
		cmd->buttons |= in_attack;
}

void aim::draw_fov() {
	if (!variables::aim::draw_fov) return;
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game()) return;
	if (!csgo::local_player) return;

	// Check if the weapon can shoot, if not we dont care about fov
	weapon_t* active_weapon = csgo::local_player->active_weapon();
	if (active_weapon && active_weapon->is_non_aim()) return;

	// Screen width and height
	int sw, sh;
	interfaces::engine->get_screen_size(sw, sh);

	// Very bad way of doing it but it's the one that works best atm. Feel free to PR.
	const float aspect_ratio = (float)sw / (float)sh;
	float aspect_ratio_fov;
	if (aspect_ratio == 16.f/9.f)			aspect_ratio_fov = 106.3f;
	else if (aspect_ratio == 16.f/10.f)		aspect_ratio_fov = 100.4f;
	else if (aspect_ratio == 4.f/3.f)		aspect_ratio_fov = 90.f;
	else if (aspect_ratio == 5.f/4.f)		aspect_ratio_fov = 86.3f;
	else									aspect_ratio_fov = 106.3f;		// 16:9 if not in list default

	// Calculate radius
	const float screen_fov = variables::misc_visuals::custom_fov_slider / 90.f * aspect_ratio_fov;
	float x1 = tan(DEG2RAD(variables::aim::aimbot_fov));
	float x2 = tan(DEG2RAD(screen_fov) / 2);
	float rad = (x1 / x2) * (sw/2);
	
	render::draw_circle(sw/2, sh/2, rad, 255, variables::colors::aimbot_fov_c);
}