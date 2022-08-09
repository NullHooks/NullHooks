#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

#ifdef _DEBUG
#include "core/features/debug/debug.hpp"
#endif // _DEBUG

// Checks if we can fire, used in other places
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
	vec3_t final_target(0, 0, 0);					// Position of best hitbox. Will be returned

	// Check if we have weapon data before doing anything else
	const auto weapon_data = active_weapon->get_weapon_data();
	if (!weapon_data) return final_target;

	// Get eye pos from localplayer
	auto local_eye_pos = csgo::local_player->get_eye_pos();

	// Players will get appended here with their fov. Then the vector will get ordered.
	std::vector<std::pair<float, player_t*>> target_list{};

	// Iterate players and store the fov to their eyes. It is just so we can have an aproximate priority by crosshair distance.
	for (int i = 1; i <= interfaces::globals->max_clients; i++) {
		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		if (!entity
			|| entity == csgo::local_player
			|| !entity->is_alive()
			|| entity->dormant()
			|| entity->has_gun_game_immunity()) continue;
		if (!helpers::is_enemy(entity) && !variables::aim::target_friends) continue;

		// We get the eye position of the current entity so we can store it, and then order by it. This doesn't have to be precise
		auto entity_pos = entity->get_eye_pos();

		// Calculate relative angle to target
		vec3_t aim_angle = math::calculate_relative_angle(local_eye_pos, entity_pos, cmd->viewangles);
		aim_angle.clamp();

		// Get the fov to target. Lower == closer to crosshair == better
		const float fov = std::hypot(aim_angle.x, aim_angle.y);

		// Push to target vector the entity with its fov
		target_list.push_back({ fov, entity });
	}

	// After storing the players, order them from lower fov to greater fov (priority)
	std::sort(target_list.begin(), target_list.end(), [](const std::pair<float, player_t*>& a, const std::pair<float, player_t*>& b) -> bool {
		return a.first < b.first;
	});

	// Store selected hitboxes
	std::vector<int> all_hitboxes = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18 };	// For bodyaim if lethal
	std::vector<int> selected_hitboxes;
	if (variables::aim::hitboxes.is_enabled(0)) {		// Head
		selected_hitboxes.emplace_back(hitbox_head);
	}
	if (variables::aim::hitboxes.is_enabled(1)) {		// Neck
		selected_hitboxes.emplace_back(hitbox_neck);
	}
	if (variables::aim::hitboxes.is_enabled(2)) {		// Chest
		selected_hitboxes.emplace_back(hitbox_pelvis);
		selected_hitboxes.emplace_back(hitbox_stomach);
		selected_hitboxes.emplace_back(hitbox_lower_chest);
		selected_hitboxes.emplace_back(hitbox_chest);
		selected_hitboxes.emplace_back(hitbox_upper_chest);
	}
	if (variables::aim::hitboxes.is_enabled(3)) {		// Arms
		selected_hitboxes.emplace_back(hitbox_right_hand);
		selected_hitboxes.emplace_back(hitbox_left_hand);
		selected_hitboxes.emplace_back(hitbox_right_upper_arm);
		selected_hitboxes.emplace_back(hitbox_left_upper_arm);
		selected_hitboxes.emplace_back(hitbox_right_forearm);
		selected_hitboxes.emplace_back(hitbox_left_forearm);

	}
	if (variables::aim::hitboxes.is_enabled(4)) {		// Legs
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
	int best_hitbox = 0;		// For debugging what hitbox type are we using
	#endif // _DEBUG

	// Iterate the ordered target list
	for (const std::pair<float, player_t*> item : target_list) {
		// Check if we are checking a target with more fov than needed
		if (item.first > variables::aim::aimbot_fov) break;

		const auto entity = item.second;
		const float entity_health = entity->health();

		// Will store the best damage and hitbox of the current player
		float best_player_damage = 0.f;
		vec3_t best_player_hitbox(0, 0, 0);

		// Iterate all possible hitboxes, even if not enabled (for checking bodyaim if lethal)
		for (const int hitbox : all_hitboxes) {
			auto hitbox_pos = entity->get_hitbox_position_fixed(hitbox);
			bool enabled_hitbox = std::find(selected_hitboxes.begin(), selected_hitboxes.end(), hitbox) != selected_hitboxes.end();		// Enabled by user

			autowall_data_t autowall_data = { false, 0.f };

			if (variables::aim::autowall.idx < 2) {
				// If "Only visible" and we can't see it, ignore hitbox
				if (variables::aim::autowall.idx == 0 && !csgo::local_player->can_see_player_pos(entity, hitbox_pos))
					continue;

				// Get autowall data and check if we can make enough damage or kill. autowall::handle_walls() takes care of stuff like "bodyaim if lethal" and "only visible",
				// so it will only return damage for valid hitboxes
				autowall_data = aim::autowall::handle_walls(csgo::local_player, entity, hitbox_pos, weapon_data, enabled_hitbox);

				#ifdef _DEBUG
				debug::points.push_back({ hitbox_pos, enabled_hitbox });
				#endif // _DEBUG

				// Check if the autowall data was invalid
				if (autowall_data.damage < 0.f)
					continue;

				// Check if the returned damage is enough or if we can kill the target (we dont need to worry about bodyaim_if_lethal here)
				if (autowall_data.damage < (int)variables::aim::min_damage && !autowall_data.lethal)
					continue;
			} else if (variables::aim::autowall.idx == 2) {
				// @todo: bodyaim_if_lethal would not work with "ignore walls" because we dont run autowall
				// We are trying to use ignore walls with disabled hitbox.
				if (!enabled_hitbox) continue;

				// We don't care about walls and we found a valid hitbox, return it
				return hitbox_pos;
			}

			// If we can kill, we don't care about any other players, since we are checking by fov priority
			if (autowall_data.lethal || autowall_data.damage >= entity_health)
				return hitbox_pos;

			// Check what the best hitbox would be based on damage, then save it as "this player's best hitbox"
			if (autowall_data.damage > best_player_damage) {
				best_player_damage = autowall_data.damage;
				best_player_hitbox = hitbox_pos;

				#ifdef _DEBUG
				best_hitbox = hitbox;		// For debugging what hitbox type are we using
				#endif // _DEBUG
			}
		}

		/*
		 * What we are doing here is saving the hitbox with the most damage from the current player (best_player_hitbox) as the final target ONLY if we don't have a final target yet (we found a closer non-lethal target).
		 * We do it this way so:
		 *		1. We check if we can do damage to the closest player
		 *		2. If we can't (after checking hitboxes, all are 0 damage) go to the next player until we find a target that we can damage inside our fov (fov is checked on the first line of the loop)
		 *		3. Once we find a valid hitbox (max damage of closest player), store it as the final target, but don't return/break inmediately
		 *		4. If we have priorize_lethal_targets, keep iterating the rest of the players in the vector to see if we can deal lethal damage to a hitbox, then return that instead (just when we find the lethal hitbox)
		 */
		if (best_player_damage > 0.f && final_target.is_zero()) {
			final_target = best_player_hitbox;

			if (!variables::aim::priorize_lethal_targets) break;
		}
	}

	#ifdef _DEBUG
	if (!final_target.is_zero()) {
		auto hitbox_str = [](int hitbox) -> std::string {
			switch (hitbox) {
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

	return final_target;		// vec3_t position of the best bone/hitbox. Zero if not found.
}

void aim::run_aimbot(c_usercmd* cmd) {
	if (!(variables::aim::autofire && input::global_input.IsHeld(variables::aim::aimbot_key.key))	// Not holding aimbot key
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
		local_aim_punch = csgo::local_player->get_aim_punch();
	} else {
		switch (weapon_data->weapon_type) {
			case WEAPONTYPE_RIFLE:
			case WEAPONTYPE_SUBMACHINEGUN:
			case WEAPONTYPE_MACHINEGUN:
				local_aim_punch = csgo::local_player->get_aim_punch();
		}
	}

	vec3_t enemy_angle = (aim_angle - local_aim_punch) - cmd->viewangles;
	enemy_angle.clamp();

	vec3_t angle_diff = enemy_angle;
	if (!variables::aim::silent) angle_diff *= (1.f - variables::aim::aimbot_smoothing);	// Scale acording to smoothing if not silent

	vec3_t final_angle = cmd->viewangles + angle_diff;		// The current angle before the aimbot + what we should move
	if (!variables::aim::silent)
		interfaces::engine->set_view_angles(final_angle);

	cmd->viewangles = final_angle;

	if (variables::aim::autofire && input::global_input.IsHeld(variables::aim::aimbot_key.key))
		cmd->buttons |= in_attack;
}

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
	float rad = (x1 / x2) * (sw / 2);

	render::draw_circle(sw / 2, sh / 2, rad, 255, variables::colors::aimbot_fov_c);
}
