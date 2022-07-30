#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

#ifdef _DEBUG
#include "core/features/debug/debug.hpp"
#endif // _DEBUG

#pragma region AIMBOT CHECKS
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
#pragma endregion

#pragma region GET TARGET
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

	#ifdef _DEBUG
	debug::points.clear();
	debug::shots.clear();
	#endif // _DEBUG

	float best_fov = variables::aim::aimbot_fov;	// This variable will store the fov of the closest player to crosshair, we start it as the fov setting
	vec3_t best_target(0, 0, 0);					// Position of best hitbox. Will be returned
	const auto weapon_data = active_weapon->get_weapon_data();
	if (!weapon_data) return best_target;

	#ifdef _DEBUG
	int best_hitbox = 0;
	#endif // _DEBUG

	// Check each player
	for (int i = 1; i <= interfaces::globals->max_clients; i++) {
		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		if (!entity
			|| entity == csgo::local_player
			|| !entity->is_alive()
			|| entity->dormant()
			|| entity->has_gun_game_immunity()) continue;
		if (!helpers::is_enemy(entity) && !variables::aim::target_friends) continue;

		auto local_eye_pos = csgo::local_player->get_eye_pos();		// Get eye pos from origin player_t

		for (const auto hitbox : all_hitboxes) {
			auto hitbox_pos = entity->get_hitbox_position(hitbox);
			bool enabled_hitbox = std::find(selected_hitboxes.begin(), selected_hitboxes.end(), hitbox) != selected_hitboxes.end();

			#ifdef _DEBUG
			debug::points.push_back({hitbox_pos, enabled_hitbox});
			#endif // _DEBUG

			// Ignore everything if we have "ignore walls" setting (2)
			if (variables::aim::autowall.idx != 2) {
				if (!aim::autowall::handle_walls(csgo::local_player, entity, hitbox_pos, weapon_data, (int)variables::aim::min_damage, enabled_hitbox))
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
				#ifdef _DEBUG
				best_hitbox = hitbox;
				#endif // _DEBUG
			}
		}
	}

	#ifdef _DEBUG
	if(!best_target.is_zero()) {
		auto hitbox_str = [](int hitbox) -> std::string {
			switch(hitbox) {
			case hitbox_head:            return "HEAD";
			case hitbox_neck:            return "NECK";
			case hitbox_pelvis:          return "PELVIS";
			case hitbox_stomach:         return "STOMACH";
			case hitbox_lower_chest:     return "LOWER CHEST";
			case hitbox_chest:           return "CHEST";
			case hitbox_upper_chest:     return "UPPER CHEST";
			case hitbox_right_hand:      return "RIGHT HAND";
			case hitbox_left_hand:       return "LEFT HAND";
			case hitbox_right_upper_arm: return "RIGHT UPPER ARM";
			case hitbox_left_upper_arm:  return "LEFT UPPER ARM";
			case hitbox_right_forearm:   return "RIGHT FOREARM";
			case hitbox_left_forearm:    return "LEFT FOREARM";
			case hitbox_right_thigh:     return "RIGHT THIGH";
			case hitbox_left_thigh:      return "LEFT THIGH";
			case hitbox_right_calf:      return "RIGHT CALF";
			case hitbox_left_calf:       return "LEFT CALF";
			case hitbox_right_foot:      return "RIGHT FOOT";
			case hitbox_left_foot:       return "LEFT FOOT";
			}
		};
		debug::log::puts(std::string("HB: ") + hitbox_str(best_hitbox));
	}
	#endif // _DEBUG

	return best_target;		// vec3_t position of the best bone
}
#pragma endregion

#pragma region ACTUAL AIMBOT
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

	vec3_t angle_diff = enemy_angle;	
	if (!variables::aim::silent) angle_diff *= (1.f - variables::aim::aimbot_smoothing);	// Scale acording to smoothing if not silent
	
	vec3_t final_angle = cmd->viewangles + angle_diff;		// The current angle before the aimbot + what we should move
	if (!variables::aim::silent)
		interfaces::engine->set_view_angles(final_angle);
	
	cmd->viewangles = final_angle;

	if (variables::aim::autofire && input::gobal_input.IsHeld(variables::aim::aimbot_key.key))
		cmd->buttons |= in_attack;
}
#pragma endregion

#pragma region FOV CIRCLE
// Used in aim::draw_fov()
float scale_fov_by_width(float fov, float aspect_ratio) {
	aspect_ratio *= 0.75f;
	
	float half_angle_rad = fov * (0.5f * M_PI / 180.0f);
	float t = tan(half_angle_rad) * aspect_ratio;
	float retDegrees = (180.0f / M_PI) * atan(t);
	
	return retDegrees * 2.0f;
}

// Used in paint_traverse
void aim::draw_fov() {
	if (!variables::aim::aimbot || !variables::aim::draw_fov) return;
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game()) return;
	if (!csgo::local_player || !csgo::local_player->is_alive()) return;

	// Check if the aimbot fov is too large to render
	if (variables::aim::aimbot_fov > 90.f) return;

	// Check if the weapon can shoot, if not we dont care about fov
	weapon_t* active_weapon = csgo::local_player->active_weapon();
	if (active_weapon && active_weapon->is_non_aim()) return;

	// Screen width and height
	int sw, sh;
	interfaces::engine->get_screen_size(sw, sh);

	// Calculate radius
	const float aspect_ratio = (float)sw / (float)sh;
	const float screen_fov = scale_fov_by_width(variables::misc_visuals::custom_fov_slider, aspect_ratio);
	float x1 = tan(DEG2RAD(variables::aim::aimbot_fov));
	float x2 = tan(DEG2RAD(screen_fov) / 2);
	float rad = (x1 / x2) * (sw/2);
	
	render::draw_circle(sw/2, sh/2, rad, 255, variables::colors::aimbot_fov_c);
}
#pragma endregion