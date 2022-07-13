#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

void misc::movement::slow_walk(c_usercmd* cmd, int old_flags) {
    if (!variables::misc::slowwalk) return;
    if (!csgo::local_player) return;
    if (!(csgo::local_player->flags() & fl_onground && old_flags & fl_onground)) return;        // We are not walking

    const auto weapon = csgo::local_player->active_weapon();
    if (!weapon) return;
    const auto weapon_info = weapon->get_weapon_data();
    if (!weapon_info) return;

    auto max_speed = 0.33f * (csgo::local_player->is_scoped() ? weapon_info->weapon_max_speed_alt : weapon_info->weapon_max_speed);     // Max slowwalk speed

    if (!aim::aimbot_weapon_check(false)) {        // Knife, grenade, taser, c4, reloading, ...
        const auto move_length = sqrt(cmd->sidemove * cmd->sidemove + cmd->forwardmove * cmd->forwardmove);
        const auto forwardmove = cmd->forwardmove / move_length;
        const auto sidemove    = cmd->sidemove / move_length;

        if (move_length > max_speed) {
            if (max_speed + 1.0f < csgo::local_player->velocity().length()) {
                cmd->forwardmove = 0.0f;
                cmd->sidemove    = 0.0f;
            } else {
                cmd->sidemove    = max_speed * sidemove;
                cmd->forwardmove = max_speed * forwardmove;
            }
        }
    } else {
        auto forwardmove = cmd->forwardmove;
        auto sidemove = cmd->sidemove;

        auto move_length = sqrt(sidemove * sidemove + forwardmove * forwardmove);
        auto move_length_backup = move_length;

        if (move_length > 110.0f) {
            cmd->forwardmove = forwardmove / move_length_backup * 110.0f;
            move_length      = sidemove / move_length_backup * 110.0f;
            cmd->sidemove    = sidemove / move_length_backup * 110.0f;
        }
    }
}