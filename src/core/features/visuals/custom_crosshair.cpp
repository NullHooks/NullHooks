#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

void visuals::crosshair::draw_custom_crosshair(int x, int y, int outline, color cross_color) {
	const int gap = (int)variables::crosshair::crosshair_gap;
	const int len = (int)variables::crosshair::crosshair_len;

	const int top = y - len - gap;
	const int bottom = y + len + gap;
	const int left = x - len - gap;
	const int right = x + len + gap;

	// Draw outline with color's opacity
	if (outline) {
		render::draw_rect(x - 1, top - 1, 3, (len + gap)*2 + 3, color::black(cross_color.a));
		render::draw_rect(left - 1, y - 1, (len + gap)*2 + 3, 3, color::black(cross_color.a));
	}
	// Draw 1px crosshair
	render::draw_filled_rect(x, top, 1, bottom - top + 1, cross_color);
	render::draw_filled_rect(left, y, right - left + 1, 1, cross_color);
}

void visuals::crosshair::custom_crosshair() {
	if (!variables::misc_visuals::crosshair) {
		if (variables::crosshair::only_engine_crosshair && !variables::crosshair::using_cs_crosshair) {
			variables::crosshair::using_cs_crosshair = true;
			interfaces::engine->execute_cmd("crosshair 1");
			helpers::console::state_to_console("Crosshair", "Using cs:go crosshair...");
		}
		return;
	}

	if (!interfaces::engine->is_in_game() || !interfaces::engine->is_connected()) return;
	if (!csgo::local_player) return;

	player_t* local_player_ent = (csgo::local_player->is_alive()) ? csgo::local_player : reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity_handle(csgo::local_player->observer_target()));
	if (!local_player_ent->is_alive()) return;
	if (local_player_ent->is_scoped()) return;

	int screen_w, screen_h;
	interfaces::engine->get_screen_size(screen_w, screen_h);

	const int mid_x = screen_w / 2;
	const int mid_y = screen_h / 2;

	if (variables::crosshair::only_engine_crosshair && variables::crosshair::using_cs_crosshair) {
		variables::crosshair::using_cs_crosshair = false;
		interfaces::engine->execute_cmd("crosshair 0");
		helpers::console::state_to_console_color("Crosshair", "Using engine crosshair...");
	}

	visuals::crosshair::draw_custom_crosshair(mid_x, mid_y, true, variables::colors::crosshair_c);
}

/* -------------------------------- RECOIL CROSSHAIR -------------------------------- */

// Same as aimbot but for recoil crosshair
bool recoil_crosshair_weapon_check() {
	weapon_t* active_weapon = csgo::local_player->active_weapon();
	if (!active_weapon) return false;
	const weapon_info_t* weapon_data = active_weapon->get_weapon_data();
	if (!weapon_data) return false;

	switch (weapon_data->weapon_type) {												// Only draw recoil crosshair on weapons that shoot
		case WEAPONTYPE_MACHINEGUN:
		case WEAPONTYPE_RIFLE:
		case WEAPONTYPE_SUBMACHINEGUN:
		case WEAPONTYPE_SHOTGUN:
		case WEAPONTYPE_SNIPER_RIFLE:
		case WEAPONTYPE_PISTOL:
			break;
		default:
			return false;
	}

	return true;
}

// TODO: Improve the aim punch and fov thing
void visuals::crosshair::recoil_crosshair() {
	if (!interfaces::engine->is_connected() && !interfaces::engine->is_in_game()) return;
	if (!csgo::local_player) return;
	if (!variables::misc_visuals::recoil_crosshair) return;

	player_t* local_player_ent = (csgo::local_player->is_alive()) ? csgo::local_player : reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity_handle(csgo::local_player->observer_target()));
	if (!local_player_ent->is_alive()) return;
	if (local_player_ent->is_scoped()) return;

	if (!recoil_crosshair_weapon_check()) return;	// Weapon does not shoot

	int screen_w, screen_h;
	interfaces::surface->get_screen_size(screen_w, screen_h);
	int x = screen_w / 2;
	int y = screen_h / 2;

	vec3_t punch = csgo::local_player->aim_punch_angle();

	// Subtract the punch from the position
	x -= (screen_w / variables::misc_visuals::custom_fov_slider) * punch.y;
	y += (screen_h / variables::misc_visuals::custom_fov_slider) * punch.x;

	// Give a margin of 1px up, down, left and right
	x = (x == screen_w / 2 - 1 || x == screen_w / 2 + 1) ? screen_w / 2 : x;
	y = (y == screen_h / 2 - 1 || x == screen_h / 2 + 1) ? screen_h / 2 : y;

	visuals::crosshair::draw_custom_crosshair(x, y, true, variables::colors::recoil_crosshair_c);
}