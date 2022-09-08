#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

void visuals::nade_predict() noexcept {
    // TODO: https://www.unknowncheats.me/forum/counterstrike-global-offensive/452030-hijacking-games-nade-prediction.html

    if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game()) return;
    if (!csgo::local_player) return;

    static auto nadeVar{ interfaces::console->get_convar("cl_grenadepreview") };
    
    nadeVar->callbacks.size = 0;
    nadeVar->set_value((variables::misc_visuals::nade_predict && !variables::antiaim::antiaim) ? 1 : 0);
}

// variables::misc::developer_csgo

void visuals::dev_mode() noexcept {

    static auto developerVar{ interfaces::console->get_convar("developer") };

    developerVar->callbacks.size = 0;
    developerVar->set_value(variables::misc::developer_csgo ? 1 : 0);
}

// variables::misc_visuals::custom_ar_slider

void visuals::aspect_ratio() noexcept {

    static auto developerVar{ interfaces::console->get_convar("r_aspectratio") };

    developerVar->callbacks.size = 0;
    developerVar->set_value(variables::misc_visuals::custom_ar_slider);
}