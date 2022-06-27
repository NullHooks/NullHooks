#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

const int speed_graph_width = 420;                          // Width in px, each px will be a speed value
static std::vector<int> speeds_vec(speed_graph_width, 0);	// Initialize vec of size graph_width with values as 0. Will store all the speed values

void shift_and_append(int new_val) {
	for (int n = 0; n < speed_graph_width - 1; n++) {		// -1 to skip last item
		speeds_vec.at(n) = speeds_vec.at(n + 1);			// Right to left shitf
	}
	speeds_vec.at(speed_graph_width - 1) = new_val;			// Add new value
}

color speed2color(int speed) {
	float_hsv hue = { speed / 600.0f, 1.0f, 1.0f };
	return custom_helpers::hsv2color(hue, 255);
}

void misc::speedgraph::update() {
	const int cur_speed = (int)std::ceil(csgo::local_player->velocity().length_2d());
	shift_and_append(cur_speed);
}

void misc::speedgraph::draw() {
	if (!variables::misc::draw_speedgraph) return;
	if (!interfaces::engine->is_in_game() || !interfaces::engine->is_connected()) return;
	if (!csgo::local_player) return;
	if (!csgo::local_player->is_alive()) return;
	if (csgo::local_player->is_scoped()) return;		// We don't want to draw speed graph when scoped

	// Render graph
	int screen_w, screen_h;
	interfaces::surface->get_screen_size(screen_w, screen_h);

	for (int n = 0; n < speeds_vec.size() - 1; n++) {   // -1 to skip last item
		int cur_speed  = speeds_vec.at(n);
		int next_speed = speeds_vec.at(n + 1);          // Needed to draw line to next value

		int cur_x  = screen_w / 2 - speed_graph_width / 2 + n;
		int next_x = cur_x + 1;
		int cur_y  = screen_h * 0.9 - cur_speed  * 0.3;
		int next_y = screen_h * 0.9 - next_speed * 0.3;

		color line_col = color::white();
		if (variables::misc::use_speedgraph_color)
			line_col = speed2color(next_speed);

		render::draw_line(cur_x, cur_y, next_x, next_y, line_col);
	}

	// Speed text
	const int cur_speed = (int)std::ceil(csgo::local_player->velocity().length_2d());
	color speed_col = speed2color(cur_speed);
	render::draw_text_string(screen_w/2, screen_h * 0.9 + 20, render::fonts::watermark_font_m, std::to_string(cur_speed), true, speed_col);
}
