#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

void visuals::misc::nade_predict() noexcept {
    // TODO: https://www.unknowncheats.me/forum/counterstrike-global-offensive/452030-hijacking-games-nade-prediction.html

    if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game()) return;
    if (!csgo::local_player) return;

    static auto nadeVar{ interfaces::console->get_convar("cl_grenadepreview") };
    
    nadeVar->callbacks.size = 0;
    nadeVar->set_value(variables::misc_visuals::nade_predict ? 1 : 0);
}