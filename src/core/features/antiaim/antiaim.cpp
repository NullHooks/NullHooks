#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

void antiaim::run_antiaim(c_usercmd* cmd, bool& sendPacket)
{
    if (!variables::antiaim::antiaim) return;

    // don't do antiaim when we are dead
    if (!csgo::local_player->is_alive()) return;

    // don't do antiaim when we are shoting
    // @todo: prepare the revolver without flicking
    if (cmd->buttons & in_attack) return;

    // don't do antiaim when we are in use (open doors, interact etc)
    if (cmd->buttons & in_use) return;

    // get active weapon
    weapon_t* active_weapon = csgo::local_player->active_weapon();
    if (!active_weapon) return;
    const auto weapon_data = active_weapon->get_weapon_data();
    if (!weapon_data) return;

    // now this is pure meme :)
    if (weapon_data->weapon_type == WEAPONTYPE_GRENADE) return;
    if (weapon_data->weapon_type == WEAPONTYPE_C4) return;

    // pitch down
    cmd->viewangles.x += variables::antiaim::pitch;

    if (sendPacket)
        cmd->viewangles.y -= variables::antiaim::yaw; //set our yaw
    else
        cmd->viewangles.y += variables::antiaim::yaw; // again but for fake

    // micromovement
    // definition: the idea of micromovement is it literally "micromoves" so the server is constantly updating your LBY to your real LBY and its not rotating towards your fake one
    if (fabsf(cmd->sidemove) < 5.0f) {
        if (cmd->buttons & in_duck) // if we are ducking make the movement bigger
            cmd->sidemove = cmd->tick_count & 1 ? 3.25f : -3.25f;
        else // else just make it normal
            cmd->sidemove = cmd->tick_count & 1 ? 1.1f : -1.1f;
    }

    // @todo: lby and fakelag
}