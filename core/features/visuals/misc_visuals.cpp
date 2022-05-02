#include "../features.hpp"

void visuals::misc::nade_predict() noexcept {
    if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game()) return;
    if (!csgo::local_player) return;

    static auto nadeVar{ interfaces::console->get_convar("cl_grenadepreview") };
    
    nadeVar->callbacks.size = 0;
    nadeVar->set_value(variables::nade_predict_bool ? 1 : 0);
}