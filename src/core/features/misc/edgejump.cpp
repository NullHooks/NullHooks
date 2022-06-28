#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

void misc::movement::edgejump(c_usercmd* cmd, int old_flags) {
	if (false) return;									// Setting
	if (!input::gobal_input.IsHeld(VK_PRIOR)) return;	// Key (PAGE UP until I add key thing to framework)
	if (!csgo::local_player) return;
	if (csgo::local_player->move_type() == movetype_ladder || csgo::local_player->move_type() == movetype_noclip) return;

	if (old_flags & fl_onground && !(csgo::local_player->flags() & fl_onground))
		cmd->buttons |= in_jump;
}