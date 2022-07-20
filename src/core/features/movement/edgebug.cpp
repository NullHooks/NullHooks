#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

void misc::movement::edgebug(c_usercmd* cmd, int old_flags) {
    if (!variables::misc::edgebug) return;
    if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game()) return;
    if (!csgo::local_player) return;
    if (!csgo::local_player->is_alive()) return;
    if (!(input::gobal_input.IsHeld(variables::misc::eb_key) && (csgo::local_player->flags() & fl_onground))) return;

    // Replace with a simpler method I understand until the legendary patoke PR
    if (!(old_flags & fl_onground) && (csgo::local_player->flags() & fl_onground))
        cmd->buttons |= in_duck;
}