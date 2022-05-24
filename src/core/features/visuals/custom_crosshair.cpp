#include "../features.hpp"

void visuals::crosshair::draw_custom_crosshair(int x, int y, int outline, color cross_color) {
	const int gap = (int)variables::crosshair::crosshair_gap;
	//const int width = (int)variables::crosshair::crosshair_w;
	const int len = (int)variables::crosshair::crosshair_len;

	const int top = y - len - gap;
	const int bottom = y + len + gap;
	const int left = x - len - gap;
	const int right = x + len + gap;

	// Draw outline
	if (outline) {
		render::draw_rect(x - 1, top - 1, 3, (len + gap)*2 + 3, color::black(255));
		render::draw_rect(left - 1, y - 1, (len + gap)*2 + 3, 3, color::black(255));
	}
	// Draw 1px crosshair
	render::draw_line(x, top, x, bottom, cross_color);
	render::draw_line(left, y, right, y, cross_color);
}

void visuals::crosshair::custom_crosshair() {
	if (!variables::crosshair_bool) {
		if (variables::crosshair::only_engine_crosshair && !variables::crosshair::using_cs_crosshair) {
			variables::crosshair::using_cs_crosshair = true;
			interfaces::engine->execute_cmd("crosshair 1");
			custom_helpers::state_to_console("Crosshair", "Using cs:go crosshair...");
		}
		return;
	}

	if (!interfaces::engine->is_in_game() || !interfaces::engine->is_connected()) return;
	if (!csgo::local_player) return;
	if (csgo::local_player->is_scoped()) return;

	int screen_w, screen_h;
	interfaces::engine->get_screen_size(screen_w, screen_h);

	const int mid_x = screen_w / 2;
	const int mid_y = screen_h / 2;

	if (variables::crosshair::only_engine_crosshair && variables::crosshair::using_cs_crosshair) {
		variables::crosshair::using_cs_crosshair = false;
		interfaces::engine->execute_cmd("crosshair 0");
		custom_helpers::state_to_console("Crosshair", "Using engine crosshair...");
	}

	visuals::crosshair::draw_custom_crosshair(mid_x, mid_y, true, (variables::crosshair::rainbow_crosshair) ? color(rand() % 255, rand() % 255, rand() % 255) : color::white(255));
}