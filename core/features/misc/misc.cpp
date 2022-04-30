#include "../features.hpp"

void misc::movement::bunny_hop(c_usercmd* cmd) {
	if (!variables::bhop_bool) return;

	const int move_type = csgo::local_player->move_type();
	if (move_type == movetype_ladder || move_type == movetype_noclip || move_type == movetype_observer) return;

	if (!(csgo::local_player->flags() & fl_onground)) cmd->buttons &= ~in_jump;
};

void misc::custom_crosshair() {

	if (!variables::crosshair_bool) {
		if (variables::crosshair::only_engine_crosshair && !variables::crosshair::using_cs_crosshair) {
			variables::crosshair::using_cs_crosshair = true;
			interfaces::engine->execute_cmd("crosshair 1");
			custom_helpers::state_to_console("Crosshair", "Using cs:go crosshair...");
		}
		return;
	}

	if (interfaces::engine->is_connected()) {
		int wa, ha;
		interfaces::engine->get_screen_size(wa, ha);

		const int mid_x = wa / 2;
		const int mid_y = ha / 2;

		if (variables::crosshair::only_engine_crosshair && variables::crosshair::using_cs_crosshair) {
			variables::crosshair::using_cs_crosshair = false;
			interfaces::engine->execute_cmd("crosshair 0");
			custom_helpers::state_to_console("Crosshair", "Using engine crosshair...");
		}

		/*
		// Draw diagonal with gap (cod hitmarker) crosshair
		render::draw_line(mid_x - 5, mid_y - 5, mid_x - 1, mid_y - 1, color::red(255));
		render::draw_line(mid_x + 5, mid_y + 5, mid_x + 1, mid_y + 1, color::red(255));
		render::draw_line(mid_x + 5, mid_y - 5, mid_x + 1, mid_y - 1, color::red(255));
		render::draw_line(mid_x - 5, mid_y + 5, mid_x - 1, mid_y + 1, color::red(255));
		*/

		render::draw_line(mid_x, mid_y - variables::crosshair::crosshair_len,
			mid_x, mid_y - variables::crosshair::crosshair_gap, color::red(255));
		render::draw_line(mid_x, mid_y + variables::crosshair::crosshair_gap,
			mid_x, mid_y + variables::crosshair::crosshair_len, color::red(255));
		render::draw_line(mid_x - variables::crosshair::crosshair_len, mid_y,
			mid_x - variables::crosshair::crosshair_gap, mid_y, color::red(255));
		render::draw_line(mid_x + variables::crosshair::crosshair_gap, mid_y,
			mid_x + variables::crosshair::crosshair_len, mid_y, color::red(255));
	}
}