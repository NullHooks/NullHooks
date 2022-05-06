#include "../features.hpp"

void visuals::crosshair::recoil_crosshair() {
	if (!interfaces::engine->is_connected() && !interfaces::engine->is_in_game()) return;
	if (!csgo::local_player || !csgo::local_player->is_alive()) return;
	if (!variables::recoil_crosshair_bool) return;
	if (csgo::local_player->is_scoped()) return;

	std::pair<int, int> screen_size;

	interfaces::surface->get_screen_size(screen_size.first, screen_size.second);
	int x = screen_size.first / 2;
	int y = screen_size.second / 2;

	vec3_t punch = csgo::local_player->aim_punch_angle();

	// Subtract the punch from the position
	x -= (screen_size.first / variables::custom_fov_slider) * punch.y;
	y += (screen_size.second / variables::custom_fov_slider) * punch.x;

	x = (x == screen_size.first / 2 - 1 || x == screen_size.first / 2 + 1) ? screen_size.first / 2 : x;
	y = (y == screen_size.second / 2 - 1 || x == screen_size.second / 2 + 1) ? screen_size.second / 2 : y;
	visuals::crosshair::draw_custom_crosshair(x, y, true, (variables::crosshair::rainbow_crosshair) ? color(rand() % 255, rand() % 255, rand() % 255) : color(255, 0, 255, 255));
}