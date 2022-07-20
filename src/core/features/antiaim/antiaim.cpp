#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

void antiaim::run_antiaim(c_usercmd* cmd, bool& sendPacket) {
    if (!variables::antiaim::antiaim) return;
    if (!csgo::local_player || !csgo::local_player->is_alive()) return;      // don't do antiaim when we are dead
    if (cmd->buttons & in_use) return;      // Don't do antiaim when we are in use (open doors, interact etc)
    const int move_type = csgo::local_player->move_type();
    if (move_type == movetype_ladder || move_type == movetype_noclip || move_type == movetype_observer) return;

    // don't do antiaim when we are shoting
    // @todo: detect when we shoot instead of click
    // @todo: prepare the revolver without flicking
    if (cmd->buttons & in_attack) return;

    // Don't aa if we are planting or trying to throw a grenade
    weapon_t* active_weapon = csgo::local_player->active_weapon();
    if (!active_weapon) return;
    if ((active_weapon->is_bomb() && cmd->buttons & in_use)
        || (active_weapon->is_grenade() && cmd->buttons & in_attack)) return;

    // Pitch down
    cmd->viewangles.x += variables::antiaim::pitch;

    if (sendPacket)
        cmd->viewangles.y -= variables::antiaim::yaw;       // Set our yaw
    else
        cmd->viewangles.y += variables::antiaim::yaw;       // Again but for fake

    // micromovement
    // definition: the idea of micromovement is it literally "micromoves" so the server is constantly updating your LBY to your real LBY and its not rotating towards your fake one
    if (fabsf(cmd->sidemove) < 5.0f) {
        if (cmd->buttons & in_duck)     // if we are ducking make the movement bigger
            cmd->sidemove = cmd->tick_count & 1 ? 3.25f : -3.25f;
        else                            // else just make it normal
            cmd->sidemove = cmd->tick_count & 1 ? 1.1f : -1.1f;
    }

    // @todo: lby and fakelag
}