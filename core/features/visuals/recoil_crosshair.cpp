#include "../features.hpp"

void visuals::crosshair::recoil_crosshair() {
	if (!interfaces::engine->is_connected() && !interfaces::engine->is_in_game()) return;
	if (!csgo::local_player || !csgo::local_player->is_alive()) return;
	if (!variables::recoil_crosshair_bool) return;

	std::pair<int, int> screen_size;

	interfaces::surface->get_screen_size(screen_size.first, screen_size.second);
	int x = screen_size.first / 2;
	int y = screen_size.second / 2;

	vec3_t punch = csgo::local_player->aim_punch_angle();
	if (csgo::local_player->is_scoped())
		punch /= .5f;

	// subtract the punch from the position
	x -= (screen_size.first / 90) * punch.y;
	y += (screen_size.second / 90) * punch.x;

	visuals::crosshair::draw_custom_crosshair(x, y, true, variables::crosshair_bool ? color::red(255) : color::white(255));
}