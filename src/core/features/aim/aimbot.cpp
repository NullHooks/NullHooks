#include "../features.hpp"

// Checks if the current weapon can shoot and all that
bool aimbot_weapon_check() {
	if (csgo::local_player->is_defusing()) return false;

	weapon_t* active_weapon = csgo::local_player->active_weapon();
	if (!active_weapon) return false;

	const int weapon_type = active_weapon->get_weapon_data()->weapon_type;
	switch (weapon_type) {										// Only aimbot on weapons that shoot
	case WEAPONTYPE_MACHINEGUN:
	case WEAPONTYPE_RIFLE:
	case WEAPONTYPE_SUBMACHINEGUN:
	case WEAPONTYPE_SHOTGUN:
	case WEAPONTYPE_SNIPER_RIFLE:
	case WEAPONTYPE_PISTOL: {
		if (!active_weapon->clip1_count()) return false;			// No ammo so don't aimbot

		if (weapon_type == WEAPONTYPE_SNIPER_RIFLE
			&& !csgo::local_player->is_scoped()
			&& !variables::aim::aimbot_noscope) return false;		// We are not scoped and have the noscope option disabled

		break;
	}
	default: return false;
	}

	// (We reached here without return so we are good to use aimbot)
	return true;
}

void aim::run_aimbot(c_usercmd* cmd) {
	if (!(cmd->buttons & cmd_buttons::in_attack)) return;		// User not attacking
	if (!variables::aim::aimbot) return;
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game()) return;
	if (!csgo::local_player) return;
	if (!aimbot_weapon_check()) return;

	vec3_t best_angle{};
	float best_fov{ variables::aim::aimbot_fov };	// This variable will store the fov of the closest player to crosshair, we start it as the fov setting

	for (int n = 1; n <= interfaces::globals->max_clients; n++) {
		auto cur_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(n));
		if (!cur_player
			|| !cur_player->is_alive()
			|| cur_player->dormant()
			|| cur_player->has_gun_game_immunity()) continue;
		if (cur_player->team() == csgo::local_player->team() && !variables::aim::target_friends) continue;

		matrix_t bones[128];
		if (!cur_player->setup_bones(bones, 128, 256, 0)) continue;

		// We need to get weapon_type for aim_punch anyway so
		weapon_t* active_weapon = csgo::local_player->active_weapon();
		if (!active_weapon) return;
		const int weapon_type = active_weapon->get_weapon_data()->weapon_type;

		vec3_t local_aim_punch{};	// Initialize at 0 because we only want aim punch with rifles
		switch (weapon_type) {
			case WEAPONTYPE_RIFLE:
			case WEAPONTYPE_SUBMACHINEGUN:
			case WEAPONTYPE_MACHINEGUN:
				local_aim_punch = csgo::local_player->aim_punch_angle();
		}

		auto local_eye_pos = csgo::local_player->get_eye_pos();		// Get eye pos from origin player_t
		
		// Try to trace ray to taget player (check visible)
		vec3_t cur_player_head = bones[8].get_origin();
		if (!csgo::local_player->can_see_player_pos(cur_player, cur_player_head)) continue;

		//static auto recoil_scale = interfaces::console->get_convar("weapon_recoil_scale")->float_value;	// Does not work :(
		vec3_t enemy_angle{ ((cur_player_head - local_eye_pos).to_angle() - cmd->viewangles) - local_aim_punch * 2.f };

		// First time checks the fov setting, then will overwrite if it finds a player that is closer to crosshair
		const float fov = std::hypot(enemy_angle.x, enemy_angle.y);
		if (fov < best_fov) {
			best_fov = fov;
			best_angle = enemy_angle;
		}
	}

	cmd->viewangles += best_angle * (1.f - variables::aim::aimbot_smoothing);	// Scale acording to smoothing
}

// https://www.unknowncheats.me/forum/counterstrike-global-offensive/129068-draw-aimbot-fov.html
// Needs fix
void aim::draw_fov() {
	if (!variables::aim::draw_fov) return;
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game()) return;
	if (!csgo::local_player) return;
	if (!aimbot_weapon_check()) return;
	
	// Screen width and height
	int sw, sh;
	interfaces::engine->get_screen_size(sw, sh);
	const int x_mid = sw / 2, y_mid = sh / 2;

	float rad = tanf((DEG2RAD(variables::aim::aimbot_fov)) / 6) / tanf(97)*x_mid;
	
	render::draw_circle(x_mid, y_mid, rad, 255, color::white());
}