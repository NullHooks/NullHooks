#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

void misc::movement::pre_pred_jumpbug(c_usercmd* cmd, int old_flags) {
    if (false) return;
    // check key
    if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game()) return;
    if (!csgo::local_player) return;
    if (csgo::local_player->move_type() == movetype_ladder || csgo::local_player->move_type() == movetype_noclip) return;

    cmd->buttons |= in_bullrush;    // Infinite duck

    if ( variables::misc::bhop && !(old_flags & fl_onground) && cmd->buttons & in_jump)
        cmd->buttons &= ~in_jump;   // Stop bhop
}

void misc::movement::post_pred_jumpbug(c_usercmd* cmd, int old_flags) {
    if (false) return;
    // check key
    if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game()) return;
    if (!csgo::local_player) return;
    if (csgo::local_player->move_type() == movetype_ladder || csgo::local_player->move_type() == movetype_noclip) return;

    if (!(old_flags & fl_onground) && (csgo::local_player->flags() & fl_onground))
        cmd->buttons |= in_duck;

    if (csgo::local_player->flags() & fl_onground)
        cmd->buttons &= ~in_jump;
}