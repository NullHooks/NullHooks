#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

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

player_t* get_best_target(c_usercmd* cmd) {
	vec3_t best_angle{};
	float best_fov{ 180.f * variables::aim::aimbot_fov };	// This variable will store the fov of the closest player to crosshair, we start it as the fov setting
	player_t* final_player = nullptr;

	for (int n = 1; n <= 64; n++) {
		auto cur_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(n));
		if (!cur_player
			|| cur_player == csgo::local_player
			|| !cur_player->is_alive()
			|| cur_player->dormant()
			|| cur_player->has_gun_game_immunity()) continue;
		if (cur_player->team() == csgo::local_player->team() && !variables::aim::target_friends) continue;
	
		auto local_eye_pos = csgo::local_player->get_eye_pos();		// Get eye pos from origin player_t

		matrix_t bones[128];
		if (!cur_player->setup_bones(bones, 128, 256, 0)) continue;
		vec3_t cur_player_head = bones[8].get_origin();

		// Try to trace ray to taget player (check visible)
		if (!variables::aim::ignore_walls && !csgo::local_player->can_see_player_pos(cur_player, cur_player_head)) continue;

		vec3_t aim_angle = math::calculate_angle(local_eye_pos, cur_player_head);
		aim_angle.clamp();

		// First time checks the fov setting, then will overwrite if it finds a player that is closer to crosshair
		const float fov = cmd->viewangles.distance_to(aim_angle);
		if (fov < best_fov) {
			best_fov = fov;
			final_player = cur_player;
		}
	}

	return final_player;
}

void aim::run_aimbot(c_usercmd* cmd) {
	if (!(cmd->buttons & cmd_buttons::in_attack)) return;		// User not attacking
	if (!variables::aim::aimbot) return;
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game()) return;
	if (!csgo::local_player) return;
	if (!aimbot_weapon_check()) return;

	player_t* target = get_best_target(cmd);
	if (!target) return;

	auto local_eye_pos = csgo::local_player->get_eye_pos();		// Get eye pos from origin player_t

	matrix_t bones[128];
	if (!target->setup_bones(bones, 128, 256, 0)) return;
	vec3_t cur_player_head = bones[8].get_origin();

	vec3_t aim_angle = math::calculate_angle(local_eye_pos, cur_player_head);
	aim_angle.clamp();

	// We need to get weapon_type for aim_punch anyway so
	weapon_t* active_weapon = csgo::local_player->active_weapon();
	if (!active_weapon) return;
	const int weapon_type = active_weapon->get_weapon_data()->weapon_type;

	vec3_t local_aim_punch{};	// Initialize at 0 because we only want aim punch with rifles
	if (variables::aim::non_rifle_aimpunch) local_aim_punch = csgo::local_player->aim_punch_angle();
	else {
		switch (weapon_type) {
			case WEAPONTYPE_RIFLE:
			case WEAPONTYPE_SUBMACHINEGUN:
			case WEAPONTYPE_MACHINEGUN:
				local_aim_punch = csgo::local_player->aim_punch_angle();
		}
	}

	//static auto recoil_scale = interfaces::console->get_convar("weapon_recoil_scale")->float_value;	// Does not work :(
	vec3_t enemy_angle = (aim_angle - (local_aim_punch * 2.f)) - cmd->viewangles;
	enemy_angle.clamp();

	cmd->viewangles += enemy_angle * (1.f - variables::aim::aimbot_smoothing);	// Scale acording to smoothing
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