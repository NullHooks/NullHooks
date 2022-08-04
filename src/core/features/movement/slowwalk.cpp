#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

void misc::movement::slow_walk(c_usercmd* cmd) {
	if (!variables::misc::slowwalk) return;
	if (!input::global_input.IsHeld(variables::misc::slowwalk_key.key)) return;
    if (!csgo::local_player) return;
    if (!(csgo::local_player->flags() & fl_onground)) return;        // We are not walking

    const auto weapon = csgo::local_player->active_weapon();
    if (!weapon) return;
    const auto weapon_info = weapon->get_weapon_data();
    if (!weapon_info) return;

    auto max_speed = 0.33f * (csgo::local_player->is_scoped() ? weapon_info->weapon_max_speed_alt : weapon_info->weapon_max_speed);     // Max slowwalk speed
	if (max_speed <= 0.f) return;

	float min_speed = (float)(std::sqrt(cmd->forwardmove*cmd->forwardmove + cmd->sidemove*cmd->sidemove + cmd->upmove*cmd->upmove));
	if (min_speed <= 0.f) return;

	if (cmd->buttons & in_duck)
		max_speed *= 2.94117647f;

	if (min_speed <= max_speed) return;

	float final_speed = max_speed / min_speed;

	cmd->forwardmove *= final_speed;
	cmd->sidemove *= final_speed;
	cmd->upmove *= final_speed;
}