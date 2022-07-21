#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

void antiaim::run_antiaim(c_usercmd* cmd, bool& send_packet) {
    if (!variables::antiaim::antiaim) return;
    if (!csgo::local_player || !csgo::local_player->is_alive()) return;
    const int move_type = csgo::local_player->move_type();
    if (move_type == movetype_ladder || move_type == movetype_noclip || move_type == movetype_observer) return;     // Ladder or noclip

    // Don't aa if we are doing any of this
    // @todo: prepare the revolver without flicking
    weapon_t* active_weapon = csgo::local_player->active_weapon();
    if (!active_weapon) return;
    if ((aim::can_fire(csgo::local_player) && cmd->buttons & in_attack)                             // We are shooting
        || (active_weapon->is_knife() && (cmd->buttons & in_attack || cmd->buttons & in_attack2))   // We are stabbing
        || (active_weapon->is_bomb() && cmd->buttons & in_attack)                                   // Planting bomb
        || cmd->buttons & in_use) return;                                                           // Interacting with door, weapon, bomb, etc.
        
    // Don't aa when throwing a nade. Not only don't aa but don't even flick. Say thank you to ma man @hBuffer
    // @todo: Good nade prediction :(
    if (active_weapon->is_grenade() && !active_weapon->pin_pulled()) {
        float throw_time = active_weapon->throw_time();
        if (throw_time > 0) {
            send_packet = false;
            return;
        }
    }

    // Pitch
    cmd->viewangles.x += variables::antiaim::pitch;

    // Yaw
    if (send_packet)
        cmd->viewangles.y -= variables::antiaim::yaw;       // Set our yaw
    else
        cmd->viewangles.y += variables::antiaim::yaw;       // Again but for fake

    /*
     * Micromovement
     *   Definition: the idea of micromovement is it literally "micromoves" so the server is constantly updating your LBY
     *   to your real LBY and its not rotating towards your fake one.
     */
    if (fabsf(cmd->sidemove) < 5.0f) {
        if (cmd->buttons & in_duck)     // If we are ducking make the movement bigger
            cmd->sidemove = cmd->tick_count & 1 ? 3.25f : -3.25f;
        else                            // Else just make it normal
            cmd->sidemove = cmd->tick_count & 1 ? 1.1f : -1.1f;
    }

    // @todo: lby and fakelag
}