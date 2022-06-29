#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

const int speed_graph_width = 420;                          // Width in px, each px will be a speed value
static std::vector<int> speeds_vec(speed_graph_width, 0);	// Initialize vec of size graph_width with values as 0. Will store all the speed values
int old_last_jumped = 0;									// Compared to newest speed to check if it has improved or not
int last_jumped = 0;										// Speed when jumping. 0 if none

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

void draw_speed_str(int x, int y, int speed, color col) {
	render::draw_text_string(x, y, render::fonts::watermark_font_m, std::to_string(speed), true, col);

	if (last_jumped > 0) {
		y += 17;
		const color jmpspeed_col = (last_jumped > old_last_jumped) ? color(0, 240, 0) : color(230, 10, 10);
		const color p_col = color::white();

		const std::string jmpspeed_txt = std::to_string(last_jumped);
		const auto converted_jmpspeed = std::wstring(jmpspeed_txt.begin(), jmpspeed_txt.end());
		const std::string p1 = "(";
		const auto c_p1 = std::wstring(p1.begin(), p1.end());
		const std::string p2 = ")";
		const auto c_p2 = std::wstring(p2.begin(), p2.end());
		const std::string full_jmpspeed_txt = p1 + std::to_string(last_jumped) + p2;		// Used to get the len
		const auto converted_full_jmpspeed = std::wstring(full_jmpspeed_txt.begin(), full_jmpspeed_txt.end());

		int width, height;
		interfaces::surface->get_text_size(render::fonts::watermark_font_m, converted_full_jmpspeed.c_str(), width, height);

		interfaces::surface->draw_text_font(render::fonts::watermark_font_m);
		interfaces::surface->draw_text_pos(x - (width / 2), y);

		interfaces::surface->set_text_color(p_col.r, p_col.g, p_col.b, p_col.a);
		interfaces::surface->draw_render_text(c_p1.c_str(), wcslen(c_p1.c_str()));
		interfaces::surface->set_text_color(jmpspeed_col.r, jmpspeed_col.g, jmpspeed_col.b, jmpspeed_col.a);
		interfaces::surface->draw_render_text(converted_jmpspeed.c_str(), wcslen(converted_jmpspeed.c_str()));
		interfaces::surface->set_text_color(p_col.r, p_col.g, p_col.b, p_col.a);
		interfaces::surface->draw_render_text(c_p2.c_str(), wcslen(c_p2.c_str()));
	}
}

void misc::speedgraph::update(c_usercmd* cmd) {
	if (csgo::local_player->move_type() == movetype_noclip || csgo::local_player->move_type() == movetype_observer) return; // Don't update speed if noclip

	const int cur_speed = (int)std::ceil(csgo::local_player->velocity().length_2d());
	shift_and_append(cur_speed);

	if (csgo::local_player->flags() & fl_onground) {
		if (cmd->buttons & in_jump && cur_speed > 0) {		// Just jumped
			old_last_jumped = last_jumped;	// Store old to compare and get color
			last_jumped = cur_speed;		// The last jumped speed
		} else {							// Reset if player walks
			old_last_jumped = 0;
			last_jumped = 0;
		}
	}
}

void misc::speedgraph::draw() {
	if (!variables::misc::draw_speedgraph) return;
	if (!interfaces::engine->is_in_game() || !interfaces::engine->is_connected()) return;
	if (!csgo::local_player) return;
	if (!csgo::local_player->is_alive()) return;
	if (csgo::local_player->move_type() == movetype_noclip || csgo::local_player->move_type() == movetype_observer) return;	// Noclip
	if (csgo::local_player->is_scoped()) return;		// We don't want to draw speed graph when scoped

	// Render graph
	int screen_w, screen_h;
	interfaces::surface->get_screen_size(screen_w, screen_h);

	for (int n = 0; n < speeds_vec.size() - 1; n++) {   // -1 to skip last item
		int cur_speed  = speeds_vec.at(n);
		int next_speed = speeds_vec.at(n + 1);          // Needed to draw line to next value

		int cur_x  = screen_w / 2 - speed_graph_width / 2 + n;
		int next_x = cur_x + 1;
		int cur_y  = screen_h * 0.9 - cur_speed  * (variables::misc::speedgraph_h / 100 * 0.5f);
		int next_y = screen_h * 0.9 - next_speed * (variables::misc::speedgraph_h / 100 * 0.5f);

		color line_col = color::white();
		if (variables::misc::use_speedgraph_color)
			line_col = speed2color(next_speed);

		render::draw_line(cur_x, cur_y, next_x, next_y, line_col);
	}

	// Speed text
	const int cur_speed = (int)std::ceil(csgo::local_player->velocity().length_2d());
	color speed_col = speed2color(cur_speed);
	draw_speed_str(screen_w / 2, screen_h * 0.9 + 20, cur_speed, speed_col);
}
