#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

void misc::movement::infinite_duck(c_usercmd* cmd) {
	if (!variables::misc::infinite_duck) return;
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game()) return;
	if (!csgo::local_player) return;

	const int move_type = csgo::local_player->move_type();
	if (move_type == movetype_ladder || move_type == movetype_noclip || move_type == movetype_observer) return;

	cmd->buttons |= in_bullrush;
};