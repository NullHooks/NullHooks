#include "../features.hpp"

void visuals::crosshair::recoil_crosshair() {
	if (!interfaces::engine->is_connected() && !interfaces::engine->is_in_game()) return;
	if (!csgo::local_player || !csgo::local_player->is_alive()) return;
	if (!variables::misc_visuals::recoil_crosshair) return;
	if (csgo::local_player->is_scoped()) return;

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

	visuals::crosshair::draw_custom_crosshair(x, y, true, color(0, 255, 0, 255));
}