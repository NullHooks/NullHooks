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

		const int pos_x = wa / 2;
		const int pos_y = ha / 2;

		if (variables::crosshair::only_engine_crosshair && variables::crosshair::using_cs_crosshair) {
			variables::crosshair::using_cs_crosshair = false;
			interfaces::engine->execute_cmd("crosshair 0");
			custom_helpers::state_to_console("Crosshair", "Using engine crosshair...");
		}


		render::draw_line(pos_x - 5, pos_y - 5, pos_x - 1, pos_y - 1, color::red(255));
		render::draw_line(pos_x + 5, pos_y + 5, pos_x + 1, pos_y + 1, color::red(255));
		render::draw_line(pos_x + 5, pos_y - 5, pos_x + 1, pos_y - 1, color::red(255));
		render::draw_line(pos_x - 5, pos_y + 5, pos_x - 1, pos_y + 1, color::red(255));
	}
}