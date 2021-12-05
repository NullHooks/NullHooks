#include "../features.hpp"

void misc::movement::bunny_hop(c_usercmd* cmd) {
	if (!variables::bhop_bool)
		return;

	const int move_type = csgo::local_player->move_type();

	if (move_type == movetype_ladder || move_type == movetype_noclip || move_type == movetype_observer)
		return;

	if (!(csgo::local_player->flags() & fl_onground))
		cmd->buttons &= ~in_jump;
};

void misc::custom_crosshair() {
	if (!variables::crosshair_bool)
		return;

	if (interfaces::engine->is_connected()) {
		int wa, ha;
		interfaces::engine->get_screen_size(wa, ha);

		int pos_x = wa / 2 - 1;
		int pos_y = ha / 2 - 1;

		interfaces::surface->set_drawing_color(181, 0, 0, 240);
		interfaces::surface->draw_filled_rectangle(pos_x, pos_y, 2, 2);
	}
}